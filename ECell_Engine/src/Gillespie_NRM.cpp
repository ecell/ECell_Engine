#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Gillespie_NRM.hpp"


std::ostream& operator<<(std::ostream& os, const Gillespie_NRM_R& _g_nrm_r)
{
	std::cout << "(";
	for (auto it = _g_nrm_r.quantities.cbegin(); it != _g_nrm_r.quantities.cend(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << ") " << std::endl;
}

void Gillespie_NRM_R::ApplyInOutBackward(int _i)
{
	short reaction_mol_idx = 0;
	for (auto it = inTable[_i].in.cbegin(); it != inTable[_i].in.cend(); ++it)
	{
		quantities.at(*it) += inTable[_i].s.at(reaction_mol_idx);
		reaction_mol_idx++;
	}

	reaction_mol_idx = 0;
	for (auto it = outTable[_i].out.cbegin(); it != outTable[_i].out.cend(); ++it)
	{
		quantities.at(*it) -= outTable[_i].s.at(reaction_mol_idx);
		reaction_mol_idx++;
	}
}

void Gillespie_NRM_R::ApplyInOutForward(int _i)
{
	short reaction_mol_idx = 0;
	for (auto it = inTable[_i].in.cbegin(); it != inTable[_i].in.cend(); ++it)
	{
		quantities.at(*it) -= inTable[_i].s.at(reaction_mol_idx);
		reaction_mol_idx++;
	}

	reaction_mol_idx = 0;
	for (auto it = outTable[_i].out.cbegin(); it != outTable[_i].out.cend(); ++it)
	{
		quantities.at(*it) += outTable[_i].s.at(reaction_mol_idx);
		reaction_mol_idx++;
	}
}

//void Gillespie_NRM_R::BuildDep( int _nbReactions)
//{
//	for (int i = 0; i < _nbReactions; i++)
//	{
//		std::vector<int> iDep;
//		std::vector<int> inOutSymDiff;
//
//		std::set_symmetric_difference(
//			inTable[i].in.begin(), inTable[i].in.end(),
//			outTable[i].out.begin(), outTable[i].out.end(),
//			std::back_inserter(inOutSymDiff));		
//
//		for (int j = 0; j < _nbReactions; j++)
//		{
//			if (j != i) //a rule is always in its own dependency so no need to check i==j
//			{
//				std::vector<int> intersection;
//				std::set_intersection(
//					inOutSymDiff.begin(), inOutSymDiff.end(),
//					inTable[j].in.begin(), inTable[j].in.end(),
//					std::back_inserter(intersection));
//
//				if (intersection.size() > 0)
//				{
//					iDep.push_back(j);
//				}
//			}
//		}
//
//		DepRow depRow(iDep);
//
//		depTable.push_back(depRow);
//	}
//}

void Gillespie_NRM_R::BuildDep(int _nbReactions, std::unordered_map<std::string, int>* _nameMap)
{
	for (int i = 0; i < _nbReactions; i++)
	{
		std::vector<int> iDep;
		std::vector<int> inOutSymDiff;

		std::set_symmetric_difference(
			inTable[i].in.begin(), inTable[i].in.end(),
			outTable[i].out.begin(), outTable[i].out.end(),
			std::back_inserter(inOutSymDiff));		

		
		for (int j = 0; j < _nbReactions; j++)
		{
			if (j != i) //a rule is always in its own dependency so no need to check i==j
			{
				std::vector<int> klSpecies;
				astEvaluator->ExtractVariables(astEvaluator->getNode(kineticLaws[j]), _nameMap, &klSpecies);
				/*std::cout << "Variables extracted for reaction " << j <<": ";
				for (auto it = klSpecies.begin(); it != klSpecies.end(); ++it)
				{
					std::cout << " " << *it;
				}
				std::cout << std::endl;*/

				std::vector<int> intersection;
				std::set_intersection(
					inOutSymDiff.begin(), inOutSymDiff.end(),
					klSpecies.begin(), klSpecies.end(),
					std::back_inserter(intersection));

				/*std::cout << "Intersection with modified species: ";
				for (auto it = intersection.begin(); it != intersection.end(); ++it)
				{
					std::cout << " " << *it;
				}
				std::cout << std::endl;*/

				if (intersection.size() > 0)
				{
					iDep.push_back(j);
				}
			}
		}

		DepRow depRow(iDep);
		depTable.push_back(depRow);
	}
}

float Gillespie_NRM_R::ComputePropensity(int _i)
{
	/*float a = parameters[_i];
	short reaction_mol_idx = 0;
	for (auto it = inTable[_i].in.cbegin();
		it != inTable[_i].in.cend();
		++it)
	{
		a *= nCr(quantities[*it], inTable[_i].s[reaction_mol_idx]);
		reaction_mol_idx++;
	}*/
	float a = astEvaluator->Evaluate(astEvaluator->getNode(kineticLaws[_i]));
	propensities[_i] = a;

	return a;
}

float Gillespie_NRM_R::Exponential(float _lambda)
{
	//float sample = random_uniform(&rng);
	//std::cout << sample << std::endl;
	return -log(random_uniform(&rng)) / _lambda;
}

void Gillespie_NRM_R::GenerateTAUs(int _nbReactions)
{
	for (int i = 0; i < _nbReactions; i++)
	{
		float a = ComputePropensity(i);
		//std::cout << "(" << i << "," << a << ")";
		tauTable.push_back({ i, Exponential(a) });
	}
	//std::cout << std::endl;
}

void Gillespie_NRM_R::Initializes(SBMLDocument* _sbmlDoc)
{
	Model* sbmlModel = _sbmlDoc->getModel();
	int nbSpecies = sbmlModel->getNumSpecies();
	int nbReactions = sbmlModel->getNumReactions();
	int nbParameters = sbmlModel->getNumParameters();
	int nbRules = sbmlModel->getNumRules();

	//Reserve space for the tables
	quantities.reserve(nbSpecies);
	parameters.reserve(nbParameters);
	inTable.reserve(nbReactions);
	outTable.reserve(nbReactions);
	kineticLaws.reserve(nbReactions);
	tauTable.reserve(nbReactions);
	propensities.reserve(nbReactions);
	t = 0.f;

	//Build the map between species' names and
	//index of appearance.
	//Also fills in the species quantities table.
	std::unordered_map<std::string, int> sp_name_idx_map;
	std::unordered_map<std::string, int*> quantities_map;
	Species* sp;
	for (int i = 0; i < nbSpecies; ++i)
	{
		sp = sbmlModel->getSpecies(i);
		sp_name_idx_map[sp->getId()] = i;
		quantities_map[sp->getId()] = &quantities[i];
		quantities.push_back(sp->getInitialAmount());
	}

	//Retrieve the parameters' values stored in the sbml file.
	//If the parameter's is not defined as constant, then look
	//for an assignement
	std::unordered_map<std::string, int> param_name_idx_map;
	std::unordered_map<std::string, float*> parameters_map;
	Parameter* param;
	for (int i = 0; i < nbParameters; i++)
	{
		param = sbmlModel->getParameter(i);
		//std::cout << "Processing parameter " << param->getId() << "; constant = " << param->getConstant() << std::endl;
		parameters[i] = NAN;
		param_name_idx_map[param->getId()] = i;
		parameters_map[param->getId()] = &parameters[i];
		if (param->getConstant())
		{
			parameters[i] = (float)param->getValue();
		}
		std::cout << "parameters" << "[" << i << "]=" << parameters[i] << std::endl;
	}

	//Processes the parameters defined through rules
	Rule* rule;
	std::queue<Rule> rulesQ;
	for (int i = 0; i < nbRules; i++)
	{
		rule = sbmlModel->getRule(i);
		if (rule->isParameter())
		{
			//std::cout << "Processing rule for parameter " << rule->getVariable() << ": " <<
			//astEvaluator->Evaluate(rule->getMath(), &parameters_map) << std::endl;
			//*parameters_map[rule->getVariable()] = astEvaluator->Evaluate(rule->getMath(), &parameters_map);
			//std::cout << "is rule " << rule->getVariable() << " sound for evaluation: " << astEvaluator->isSound(rule->getMath(), &parameters_map) << std::endl;
			if (astEvaluator->isSound(rule->getMath(), &parameters_map))
			{
				parameters[param_name_idx_map[rule->getVariable()]] = astEvaluator->Evaluate(rule->getMath(), &parameters_map);
			}
			else
			{
				rulesQ.push(*rule);
			}
		}
	}

	int nbRulesQ = rulesQ.size();
	while (nbRulesQ > 0)
	{
		if (astEvaluator->isSound(rulesQ.front().getMath(), &parameters_map))
		{
			parameters[param_name_idx_map[rulesQ.front().getVariable()]] = astEvaluator->Evaluate(rulesQ.front().getMath(), &parameters_map);
			nbRulesQ--;
		}
		else
		{
			rulesQ.push(rulesQ.front());
		}
		rulesQ.pop();
	}

	//Fill the In and Out tables
	Reaction* r;
	KineticLaw* kl;
	int nbReactants;
	int nbProducts;
	for (int i = 0; i < nbReactions; ++i)
	{
		r = sbmlModel->getReaction(i);
		propensities.push_back(0);

		nbReactants = r->getNumReactants();
		std::vector<int> in;
		in.reserve(nbReactants);
		std::vector<int> inS;
		inS.reserve(nbReactants);
		for (int j = 0; j < nbReactants; ++j)
		{
			in.push_back(sp_name_idx_map[r->getReactant(j)->getSpecies()]);
			inS.push_back(r->getReactant(j)->getStoichiometry());
		}

		kl = r->getKineticLaw();
		//astEvaluator->PrettyPrintTree(kl->getMath(), 0);
		int root = astEvaluator->Initializes(kl->getMath());
		kineticLaws.push_back(root);
		//kineticLaws[i] = astEvaluator->Initializes(kl->getMath());
		//astEvaluator->PrettyPrintTree(kineticLaws[i],0);

		astEvaluator->ReduceVariables(astEvaluator->getNode(root), &parameters_map);
		astEvaluator->ReduceVariables(astEvaluator->getNode(root), &quantities_map);
		//std::cout << "Test evaluation of the kinetic law upon building: " << astEvaluator->PrettyPrintFormula(astEvaluator->getNode(root)) << "=" << astEvaluator->Evaluate(astEvaluator->getNode(root)) << std::endl;
		//kineticLaws.push_back(klMath);

		inTable.push_back(InRow(&in, &inS));

		nbProducts = r->getNumProducts();
		std::vector<int> out;
		out.reserve(nbProducts);
		std::vector<int> outS;
		outS.reserve(nbReactants);
		for (int j = 0; j < nbProducts; ++j)
		{
			out.push_back(sp_name_idx_map[r->getProduct(j)->getSpecies()]);
			outS.push_back(r->getProduct(j)->getStoichiometry());
		}
		outTable.push_back(OutRow(&out, &outS));
	}

	// for debug
	/*std::cout << std::endl;
	for (int i = 0; i < nbReactions; i++)
	{
		astEvaluator->PrettyPrintTree(astEvaluator->getNode(kineticLaws[i]), 0);
		std::cout << "Test evaluation of the kinetic law after building: " << 
			astEvaluator->PrettyPrintFormula(astEvaluator->getNode(kineticLaws[i])) << "=" << 
			astEvaluator->Evaluate(astEvaluator->getNode(kineticLaws[i])) << std::endl;
	}*/

	//Build the data structure representing the dependency graph.
	BuildDep(nbReactions, &sp_name_idx_map);

	//Initializes the reversible Random Number Generator
	init_state(123, &rng);

	//Draw the first reaction times
	GenerateTAUs(nbReactions);

	//Builds the reaction times indexed priority queue (indexed Minheap)
	itmh.Initialize(&tauTable);

	trace.reserve(1000);
}

void Gillespie_NRM_R::ManageTrace()
{
	if (traceBlockSize >= 1000)
	{
		trace.reserve(1000);
		traceBlockSize = 0;
	}
}

void Gillespie_NRM_R::RunForward(float _targetTime)
{
	//step 2 & 3
	std::pair<int, float> muTau = itmh.GetRoot();

	while (muTau.second < _targetTime)
	{
		//std::cout << std::endl;
		//std::cout << itmh;

		//std::cout << "Tau Before:" << muTau.second << " a Before:" << propensities[muTau.first] << std::endl;
		//std::cout << "Quantities Before: ";
		//for (int q : quantities){std::cout << q << " ";}
		//std::cout << std::endl;
		//std::cout << "Firing Rule number: " << muTau.first << std::endl;
		//step 4
		ApplyInOutForward(muTau.first);
		t = muTau.second;
		ManageTrace();
		trace.push_back(muTau.first);
		traceBlockSize++;

		//std::cout << "Quantities After: ";
		//for (int q : quantities) { std::cout << q << " "; }
		//std::cout << std::endl;

		//step 5.c: To avoid an "if" in the for loop
		//we update tau of the current reaction here.
		
		float a_new = ComputePropensity(muTau.first);
		float new_tau = Exponential(a_new) + muTau.second;
		//std::cout << "Tau After:" << new_tau << " a After:" << a_new <<std::endl;
		itmh.SetTauInRoot(new_tau);
		itmh.UpdateRoot();

		
		//step 5 (*it is alpha in the algorithm)
		for (auto it = depTable[muTau.first].dep.cbegin(); it != depTable[muTau.first].dep.cend(); ++it)
		{
			//std::cout << "Updating dependency: " << *it << std::endl;
			float a_old = propensities[*it];
			float tau_old = itmh.GetTauFromPointer(*it);

			//Step 5.a
			a_new = ComputePropensity(*it);

			//Step 5.b
			new_tau = (a_old / a_new) * (tau_old - t) + t;
			itmh.SetTauFromPointer(*it, new_tau);
			itmh.UpdateFromPointer(*it, tau_old);
		}
		//we actualize step 2 & 3
		muTau = itmh.GetRoot();
	}
}

void Gillespie_NRM_R::RunBackward(float _targetTime)
{
	int traceSize = trace.size();
	while (t > _targetTime && traceSize > 0)
	{
		//std::cout << std::endl;
		//std::cout << itmh;
		int mu_n = trace.back();
		float tau_n = itmh.GetTauFromPointer(mu_n);
		trace.pop_back();
		traceSize--;
		float a_n = propensities[mu_n];

		//std::cout << "Tau Before:" << tau_n << " a Before:" << a_n << std::endl;
		//std::cout << "Quantities Before: ";
		//for (int q : quantities) { std::cout << q << " "; }
		//std::cout << std::endl;

		//std::cout << "Firing back Rule number: " << mu_n << std::endl;

		ApplyInOutBackward(mu_n);
		//std::cout << "Quantities After: ";
		//for (int q : quantities) { std::cout << q << " "; }
		//std::cout << std::endl;
		float a_nm1 = ComputePropensity(mu_n);
		float tau_nm1 = tau_n - Exponential(a_n);
		//std::cout << "Tau After:" << tau_nm1 << " a After:" << a_nm1 << std::endl;

		t = tau_nm1;
		itmh.SetTauFromPointer(mu_n, t);
		itmh.UpdateFromPointer(mu_n, tau_n);

		for (auto it = depTable[mu_n].dep.cbegin(); it != depTable[mu_n].dep.cend(); ++it)
		{
			//std::cout << "Updating dependency: " << *it << std::endl;
			a_n = propensities[*it];
			tau_n = itmh.GetTauFromPointer(*it);
			
			//propensity at step n-1
			a_nm1 = ComputePropensity(*it);
			//tau at step n-1
			tau_nm1 = (a_n / a_nm1) * (tau_n - t) + t;

			itmh.SetTauFromPointer(*it, tau_nm1);
			itmh.UpdateFromPointer(*it, tau_n);
		}
	}

	std::cout << std::endl;
	std::cout << itmh;
}