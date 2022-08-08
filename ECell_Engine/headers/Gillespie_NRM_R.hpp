#include <iostream>
#include <vector>
#include <math.h>
#include <queue>
#include <sbml/SBMLTypes.h>
extern "C" {
#include "revrand.h"
}
#include "Gillespie_NRM_R_Data.hpp"
#include "MathUtility.hpp"
#include "ASTNode_parser.hpp"

/*
@brief Gillespie Next Reaction Method Reversed.
@details Implements the Next Reaction Method [Gibson M. &
		 Bruck J., DOI:10.1021/jp993732q] of the Gillespie
		 algorithm for exact stochastic simulation of chemical
		 reactions. [Gillespie D., DOI:10.1016/0021-9991(76)90041-3].
		 We adapted it to be able to play the simulation
		 backward (hence, Reversed). There is no big trick: we
		 simply keep a trace of which reaction was fired at what
		 time when played forward and reverse the calculation
		 from there.
*/
class Gillespie_NRM_R
{
private:
	// With i the index of an element (i.e. species) in the vector "quantities", 
	// quantities[i] is the index of the ASTNodeEx in "ASTEvaluator.formulasNodes"
	// containing the value of the species. This is a shortcut table when we need
	// the quantity of species i outside of the evaluation table.
	std::vector<int> quantities;

	// The inTable contains the information about every species mentioned as
	// reactants in a reaction. There should be as many elements in inTable as there
	// are reactions.
	std::vector<InRow> inTable;


	//std::vector<float> parameters;

	// With i the index of an element (i.e. kinetic law) in the vector "kineticLaws", 
	// kineticLaws[i] is the index of the root ASTNodeEx in "ASTEvaluator.formulasNodes"
	// from where we can evaluate the value of the kineticLaw.
	std::vector<int> kineticLaws;

	// A shortcut table storing the results of the evaluation of the kineticLaws
	std::vector<float> propensities;

	// The outTable contains the information about every species mentioned as
	// products in a reaction. There should as many elements in outTable as there
	// are reactions.
	std::vector<OutRow> outTable;

	// The table containing the information about formulas dependencies on species.
	// The entry i in depTable gives us the index of root ASTNodeEx in 
	// "ASTEvaluator.formulasNodes" corresponding to the kineticLaws (i.e. propensities)
	// that must be updated since they depend on species which quantities were just changed
	// after a reaction was trigerred.
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
	//void BuildDep(int _nbReactions);
	void BuildDep(int _nbReactions, std::unordered_map<std::string, int>* _namesMap);

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
	ASTEvaluator* astEvaluator;

	friend std::ostream& operator<<(std::ostream& os, const Gillespie_NRM_R& _g_nrm_r);

	Gillespie_NRM_R()
	{
		astEvaluator = new ASTEvaluator();
	}

	Gillespie_NRM_R(Gillespie_NRM_R& _g_nrm_r) :astEvaluator(_g_nrm_r.astEvaluator), quantities(_g_nrm_r.quantities)
	{
		astEvaluator = new ASTEvaluator(*_g_nrm_r.astEvaluator);
	};

	~Gillespie_NRM_R()
	{
		//std::cout << "Destructor of Gillespie_NRM_R called" << std::endl;
		delete astEvaluator;
	}

	/// <summary>
	/// Sets up the data structures for the simulation to run.
	/// In this version everything is hard-coded inside the method.
	/// </summary>
	/// <param name="_nbMolecules">Number of molecules in the system</param>
	/// <param name="_nbReactions">Number of reactions in the system</param>
	/// <param name="_rng_seed">Seed of the reversible rng.</param>
	//void Initializes(int _nbMolecules, int _nbReactions, unsigned long _rng_seed);

	/// <summary>
	/// Sets up the data structures for the simulation to run.
	/// </summary>
	/// <param name="_sbmlDoc">The pointer to the SBMLDocument encoding the
	/// model we wish to use.</param>
	void Initializes(SBMLDocument* _sbmlDoc);

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