#pragma once

#include <cstddef>

#include "Data/IndexedMinHeap.hpp"
#include "Data/Reaction.hpp"
#include "Maths/ReversibleRNG.hpp"
#include "Solver/BiochemicalSolver.hpp"

namespace ECellEngine::Solvers
{
	class GillespieNRMRSolver : public BiochemicalSolver
	{
	private:
		ECellEngine::Data::IndexedMinHeap tauIMH;
		ReversibleRNG rng;
		std::unordered_multimap<std::size_t, std::size_t> reactionsDependanceGraph;//one reaction idx maps to multiple reaction idx
		std::vector<unsigned short int> trace;

		void ApplyBackward(const std::string& _reactionName);

		void ApplyForward(const std::string& _reactionName);

		void BuildDependancyGraph(const std::vector<std::string>& _reactions);

		const float ComputeReactionPropensity(const std::string& _reactionName);

		void SolveBackward(const float& targetTime);

		void SolveForward(const float& targetTime);
	
	public:
		GillespieNRMRSolver(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			BiochemicalSolver(_dataState, _name)
		{
			
		}

		virtual void Initialize(const ECellEngine::Data::Module*) override;

		virtual void Update(const ECellEngine::Core::Timer& _timer) override;
	};
}