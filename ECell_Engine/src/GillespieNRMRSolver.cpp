#include "BiochemicalModule.hpp"
#include "GillespieNRMRSolver.hpp"

void ECellEngine::Solvers::GillespieNRMRSolver::ApplyBackward(const std::string& _reactionName)
{
	ECellEngine::Data::Reaction* reaction = dataState.GetReaction(_reactionName);
	//Decrementing quantities of all Products by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetProducts()->begin(); it != reaction->GetProducts()->end(); it++)
	{
		dataState.GetSpecies(*it)->Decrement(1.f);
	}

	//Incrementing quantities of all reactants by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetReactants()->begin(); it != reaction->GetReactants()->end(); it++)
	{
		dataState.GetSpecies(*it)->Increment(1.f);
	}
}

void ECellEngine::Solvers::GillespieNRMRSolver::ApplyForward(const std::string& _reactionName)
{
	ECellEngine::Data::Reaction* reaction = dataState.GetReaction(_reactionName);
	
	//Decrementing quantities of all reactants by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetReactants()->begin(); it != reaction->GetReactants()->end(); it++)
	{
		dataState.GetSpecies(*it)->Decrement(1.f);
	}

	//Incrementing quantities of all products by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetProducts()->begin(); it != reaction->GetProducts()->end(); it++)
	{
		dataState.GetSpecies(*it)->Increment(1.f);
	}
}

void ECellEngine::Solvers::GillespieNRMRSolver::BuildDependancyGraph(const std::vector<std::string>& _reactions)
{

	std::unordered_map<std::string, std::size_t> nameToIdx;
	size_t idx = 0;
	for (std::vector<std::string>::const_iterator itReactions = _reactions.begin(); itReactions != _reactions.end(); itReactions++)
	{
		nameToIdx.emplace(*itReactions, idx);
		idx++;
	}

	idx = 0;
	const std::vector<std::string>* species;
	for (std::vector<std::string>::const_iterator itReactions = _reactions.begin(); itReactions != _reactions.end(); itReactions++)
	{
		species = dataState.GetReaction(*itReactions)->GetProducts();
		for (std::vector<std::string>::const_iterator itSpecies = species->begin(); itSpecies != species->end(); itSpecies++)
		{
			auto reactionLinks = dataState.GetOperandsToOperations().equal_range(*itSpecies);
			for (auto itDependantReaction = reactionLinks.first; itDependantReaction != reactionLinks.second; itDependantReaction++)
			{
				auto s1 = nameToIdx.find((*itDependantReaction).second);
				if (s1 != nameToIdx.end())
				{
					reactionsDependanceGraph.emplace(idx, s1->second);
				}
			}
		}
		
		species = dataState.GetReaction(*itReactions)->GetReactants();
		for (std::vector<std::string>::const_iterator itSpecies = species->begin(); itSpecies != species->end(); itSpecies++)
		{
			auto reactionLinks = dataState.GetOperandsToOperations().equal_range(*itSpecies);
			for (auto itDependantReaction = reactionLinks.first; itDependantReaction != reactionLinks.second; itDependantReaction++)
			{
				auto s1 = nameToIdx.find((*itDependantReaction).second);
				if (s1 != nameToIdx.end())
				{
					reactionsDependanceGraph.emplace(idx, s1->second);
				}
			}
		}

		idx++;
	}
}

const float ECellEngine::Solvers::GillespieNRMRSolver::ComputeReactionPropensity(const std::string& _reactionName)
{
	return dataState.GetReaction(_reactionName)->ComputeKineticLaw();
}

void ECellEngine::Solvers::GillespieNRMRSolver::SolveBackward(const float& _targetTime)
{

}

void ECellEngine::Solvers::GillespieNRMRSolver::SolveForward(const float& _targetTime)
{
	//step 2 & 3
	std::pair<int, float> muTau = tauIMH.GetRoot();
	//std::cout << "Before loop | (target time, t): (" << _targetTime << ", " << t << ")." << std::endl;
	while (muTau.second < _targetTime)
	{
		//std::cout << std::endl;
		//std::cout << itmh;

		//std::cout << "Tau Before:" << muTau.second << " a Before:" << propensities[muTau.first] << std::endl;
		//std::cout << "Quantities Before: ";
		//for (int q : quantities){std::cout << q << " ";}
		//std::cout << std::endl;
		//std::cout << "Firing Rule number: " << muTau.first << " at time: " << muTau.second << std::endl;
		//step 4
		//std::cout << "astEvaluator address " << this->astEvaluator << std::endl;
		//std::cout << "astEvaluator formulaNodes has " << astEvaluator->getNbNodes() << " nodes" << std::endl;
		//std::cout << "&itmh: " << &itmh << " nodes" << std::endl;
		//std::cout << "&tauTable: " << &tauTable << " nodes" << std::endl;
		//int test = 0;

		ApplyForward(module->GetReaction(muTau.first));
		//ManageTrace();
		trace.push_back(muTau.first);
		//traceBlockSize++;
		//traceSize++;

		//std::cout << "Quantities After: ";
		//for (int q : quantities) { std::cout << q << " "; }
		//std::cout << std::endl;

		//step 5.c: To avoid an "if" in the for loop
		//we update tau of the current reaction here.

		float a_new = ComputeReactionPropensity(module->GetReaction(muTau.first));
		float new_tau = rng.Exponential(a_new) + muTau.second;
		//std::cout << "We computed values for step n+1 (Tau, a): (" << new_tau << ", " << a_new << ")" << std::endl;
		tauIMH.SetValueInRoot(new_tau);
		tauIMH.UpdateHeapFromRoot();

		auto depReactions = reactionsDependanceGraph.equal_range(muTau.first);
		//step 5 (*it is alpha in the algorithm)
		for (auto it = depReactions.first; it != depReactions.second; ++it)
		{
			//std::cout << "Updating dependency: " << *it << std::endl;
			float a_old = dataState.GetReaction(module->GetReaction((*it).second))->GetKineticLawValue();
			float tau_old = tauIMH.GetValueAtIndex((*it).second);

			//Step 5.a
			a_new = ComputeReactionPropensity(module->GetReaction((*it).second));

			//Step 5.b
			new_tau = (a_old / a_new) * (tau_old - muTau.second) + muTau.second; //muTau.second is current time
			tauIMH.SetValueAtIndex((*it).second, new_tau);
			tauIMH.UpdateHeapFrom((*it).second, tau_old);
		}
		//we actualize step 2 & 3
		muTau = tauIMH.GetRoot();
	}
}

void ECellEngine::Solvers::GillespieNRMRSolver::Initialize(const ECellEngine::Data::Module* _module)
{
	ECellEngine::Solvers::BiochemicalSolver::Initialize(_module);
	
	//Build the data structure representing the dependency graph.
	BuildDependancyGraph(module->GetAllReaction());

	//Initializes the reversible Random Number Generator
	rng.Initialize(123);

	//Draw the first reaction times (named tau in the original algorithm)
	std::vector<std::pair<std::size_t, float>> taus;
	size_t idx = 0;
	for (std::vector<std::string>::const_iterator it = module->GetAllReaction().begin(); it != module->GetAllReaction().end(); it++)
	{
		taus.push_back(std::pair<std::size_t, float>(idx, rng.Exponential(dataState.GetReaction(*it)->ComputeKineticLaw())));
		idx++;
	}

	//Builds the reaction times indexed priority queue (indexed Minheap)
	tauIMH.Initialize(taus);

	trace.reserve(1024);
}

void ECellEngine::Solvers::GillespieNRMRSolver::Update(const ECellEngine::Core::Timer& _timer)
{
	SolveForward(_timer.elapsedTime);
}
