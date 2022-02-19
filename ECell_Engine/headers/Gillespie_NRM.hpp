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
	std::vector<short int> trace;
	IndexedTauMinHeap itmh;
	rng_state rng;

	/// <summary>
	/// Updates the <see cref="inkTable"/> and <see cref="outTable"/> according
	/// to the rule of index <paramref name="_i"/> applied in the backward way.
	/// </summary>
	/// <param name="_i">Index of the rule</param>
	void ApplyInOutBackward(int _i);

	/// <summary>
	/// Updates the <see cref="inkTable"/> and <see cref="outTable"/> according
	/// to the rule of index <paramref name="_i"/> applied in the forward way.
	/// </summary>
	/// <param name="_i">Index of the rule</param>
	void ApplyInOutForward(int _i);

	/// <summary>
	/// Builds the dependency graph G mentioned in the Next Reaction Method.
	/// The dependencies are encoded in the <see cref="depTable"/>.
	/// </summary>
	/// <param name="_nbReactions">The number of reactions on which the algorithm
	/// will run.</param>
	void BuildDep(int _nbReactions);

	/// <summary>
	/// Computes the propensity of reaction <paramref name="_i"/>.
	/// </summary>
	/// <param name="_i"></param>
	/// <returns>The propensity that was just computed</returns>
	/// <remarks>The propensity is also added in the <see cref="propensities"/> vector.</remarks>
	float ComputePropensity(int _i);

	/// <summary>
	/// Samples a number according to an exponential distribution of
	/// parameter <paramref name="_lambda"/>.
	/// </summary>
	/// <param name="_lambda"></param>
	float Exponential(float _lambda);

	/// <summary>
	/// Initializes the reactions firing times.
	/// The timings are stored in the <see cref="tauTable"/>
	/// </summary>
	/// <param name="_nbReactions"></param>
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

		trace.reserve(1000);

		RunForward(0.1f);

		std::cout << std::endl;
		reverse(&rng);

		RunBackward(0.f);

		//std::cout << itmh;
	}

	/// <summary>
	/// Runs the Gillespie algorithm (Next Generation Method) in the forward way.
	/// </summary>
	/// <param name="_targetTime">The max time until which we simulate.</param>
	void RunForward(float _targetTime);

	/// <summary>
	/// Runs the Gillespie algorithm (Next Generation Method) in the backward way.
	/// </summary>
	/// <param name="_targetTime">The max time until which we simulate.</param>
	void RunBackward(float _targetTime);

};