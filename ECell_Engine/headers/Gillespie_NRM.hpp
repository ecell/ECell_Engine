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

	std::vector<short int> trace;
	short int traceBlockSize = 0;
	
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

	/// <summary>
	/// Manages reaction trace memory allocation.
	/// </summary>
	void ManageTrace();

public:
	float t;//Gillespie simulation time.

	friend std::ostream& operator<<(std::ostream& os, const Gillespie_NRM_R& _g_nrm_r);

	/// <summary>
	/// Sets up the data structures for the simulation to run.
	/// </summary>
	/// <param name="_nbMolecules">Number of molecules in the system</param>
	/// <param name="_nbReactions">Number of reactions in the system</param>
	/// <param name="_rng_seed">Seed of the reversible rng.</param>
	void Initializes(int _nbMolecules, int _nbReactions, unsigned long _rng_seed);

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