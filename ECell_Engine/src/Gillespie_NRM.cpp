#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Gillespie_NRM.hpp"


void Gillespie_NRM_R::ApplyInOutBackward(int _i)
{
	for (auto it = inkTable[_i].in->cbegin(); it != inkTable[_i].in->cend(); ++it)
	{
		quantities.at(*it)++;
	}

	for (auto it = outTable[_i].out->cbegin(); it != outTable[_i].out->cend(); ++it)
	{
		quantities.at(*it)--;
	}
}

void Gillespie_NRM_R::ApplyInOutForward(int _i)
{
	for (auto it = inkTable[_i].in->cbegin(); it != inkTable[_i].in->cend(); ++it)
	{
		quantities.at(*it)--;
	}

	for (auto it = outTable[_i].out->cbegin(); it != outTable[_i].out->cend(); ++it)
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
			inkTable[i].in->begin(), inkTable[i].in->end(),
			outTable[i].out->begin(), outTable[i].out->end(),
			std::back_inserter(inOutSymDiff));		

		for (int j = 0; j < _nbReactions; j++)
		{
			if (j != i)
			{
				std::vector<int> intersection;
				std::set_intersection(
					inOutSymDiff.begin(), inOutSymDiff.end(),
					inkTable[j].in->begin(), inkTable[j].in->end(),
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

	for (auto it = inkTable[_i].in->cbegin();
		it != inkTable[_i].in->cend();
		++it)
	{
		a *= quantities[*it];
	}

	propensities[_i] = a;

	return a;
}

float Gillespie_NRM_R::Exponential(float _lambda)
{
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

void Gillespie_NRM_R::RunForward(float _targetTime)
{
	//step 2 & 3
	std::pair<int, float> muTau = itmh.GetRoot();

	while (muTau.second < _targetTime)
	{
		std::cout << itmh;
		//step 4
		ApplyInOutForward(muTau.first);
		t = muTau.second;

		//step 5.c: To avoid an "if" in the for loop
		//we update tau of the current reaction here.
		float a_new = ComputePropensity(muTau.first);
		float new_tau = Exponential(a_new) + muTau.second;
		itmh.SetTauInRoot(new_tau);
		itmh.UpdateRoot();

		std::cout << itmh;

		std::cout << "Firing Rule number: " << muTau.first << std::endl;
		//step 5 (*it is alpha in the algorithm)
		for (auto it = depTable[muTau.first].dep.cbegin(); it != depTable[muTau.first].dep.cend(); ++it)
		{
			std::cout << "Updating dependency: " << *it << std::endl;
			float a_old = propensities[*it];
			float tau_old = itmh.GetTauFromPointer(*it);

			//Step 5.a
			a_new = ComputePropensity(*it);

			//Step 5.b
			new_tau = (a_old / a_new) * (tau_old - t) + t;
			itmh.SetTauFromPointer(*it, new_tau);
			itmh.UpdateFromPointer(*it, tau_old);
		}

		//std::cout << itmh << std::endl;

		//we actualize step 2 & 3
		muTau = itmh.GetRoot();
	}

}