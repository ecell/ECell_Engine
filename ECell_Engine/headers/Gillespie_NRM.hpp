#include <iostream>
#include <vector>
#include <math.h>
extern "C" {
#include "revrand.h"
}
#include "Gillespie_NRM_Data.hpp"

class Gillespie_NRM_R
{
private:
	std::vector<int> quantities;
	std::vector<InkRow> inkTable;
	std::vector<float> propensities;
	std::vector<OutRow> outTable;
	std::vector<DepRow> depTable;
	std::vector<std::pair<int, float>> tauTable;
	IndexedTauMinHeap itmh;
	rng_state rng;

	void BuildDep(int _nbReactions);

	float ComputePropensity(int _i);

	float Exponential(float _lambda);

	void GenerateTAUs(int _nbReactions);

public:
	float t;

	Gillespie_NRM_R(int _nbMolecules, int _nbReactions, unsigned long _rng_seed)
	{
		quantities = {6,14,8,12,9,3,5};

		std::vector<int> in0 = { 0,1 };
		std::vector<int> in1 = { 1,2 };
		std::vector<int> in2 = { 3,4 };
		std::vector<int> in3 = { 5 };
		std::vector<int> in4 = { 4,6 };

		propensities = { 0,0,0,0,0 };

		inkTable = { InkRow(&in0, 1), InkRow(&in1, 1), InkRow(&in2, 1),
					 InkRow(&in3, 1), InkRow(&in4, 1)};

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

		RunForward(0.1f);
		//std::cout << itmh;
	}

	void ApplyInOutBackward(int _i);

	void ApplyInOutForward(int _i);

	void RunForward(float _targetTime);

};