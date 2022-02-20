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
	for (auto it = inkTable[_i].in.cbegin(); it != inkTable[_i].in.cend(); ++it)
	{
		quantities.at(*it)++;
	}

	for (auto it = outTable[_i].out.cbegin(); it != outTable[_i].out.cend(); ++it)
	{
		quantities.at(*it)--;
	}
}

void Gillespie_NRM_R::ApplyInOutForward(int _i)
{
	for (auto it = inkTable[_i].in.cbegin(); it != inkTable[_i].in.cend(); ++it)
	{
		quantities.at(*it)--;
	}

	for (auto it = outTable[_i].out.cbegin(); it != outTable[_i].out.cend(); ++it)
	{
		quantities.at(*it)++;
	}
}

void Gillespie_NRM_R::BuildDep( int _nbReactions)
{
	for (int i = 0; i < _nbReactions; i++)
	{
		std::vector<int> iDep;
		std::vector<int> inOutSymDiff;

		std::set_symmetric_difference(
			inkTable[i].in.begin(), inkTable[i].in.end(),
			outTable[i].out.begin(), outTable[i].out.end(),
			std::back_inserter(inOutSymDiff));		

		for (int j = 0; j < _nbReactions; j++)
		{
			if (j != i) //a rule is always in its own dependency so no need to check i==j
			{
				std::vector<int> intersection;
				std::set_intersection(
					inOutSymDiff.begin(), inOutSymDiff.end(),
					inkTable[j].in.begin(), inkTable[j].in.end(),
					std::back_inserter(intersection));

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
	float a = inkTable[_i].k;

	for (auto it = inkTable[_i].in.cbegin();
		it != inkTable[_i].in.cend();
		++it)
	{
		a *= quantities[*it];
	}

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

void Gillespie_NRM_R::Initializes(int _nbMolecules, int _nbReactions, unsigned long _rng_seed)
{
	quantities = { 6,14,8,12,9,3,5 };

	std::vector<int> in0 = { 0,1 };
	std::vector<int> in1 = { 1,2 };
	std::vector<int> in2 = { 3,4 };
	std::vector<int> in3 = { 5 };
	std::vector<int> in4 = { 4,6 };

	propensities = { 0,0,0,0,0 };

	inkTable = { InkRow(&in0, 1), InkRow(&in1, 1), InkRow(&in2, 1),
				 InkRow(&in3, 1), InkRow(&in4, 1) };

	std::vector<int> OutRow0 = { 2 };
	std::vector<int> OutRow1 = { 3 };
	std::vector<int> OutRow2 = { 4,5 };
	std::vector<int> OutRow3 = { 3,6 };
	std::vector<int> OutRow4 = { 0 };

	outTable = { OutRow(&OutRow0),OutRow(&OutRow1),OutRow(&OutRow2),
				 OutRow(&OutRow3),OutRow(&OutRow4) };

	t = 0.f;

	BuildDep(_nbReactions);

	tauTable.reserve(_nbReactions);
	init_state(_rng_seed, &rng);
	GenerateTAUs(_nbReactions);

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