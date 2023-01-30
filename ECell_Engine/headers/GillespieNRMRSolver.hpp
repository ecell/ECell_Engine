#pragma once

#include <cstddef>

#include "BiochemicalSolver.hpp"
#include "IndexedMinHeap.hpp"
#include "Reaction.hpp"
#include "ReversibleRNG.hpp"

namespace ECellEngine::Solvers
{
	class GillespieNRMRSolver : public BiochemicalSolver
	{
	private:
		
		ECellEngine::Data::IndexedMinHeap tauIMH;
		ReversibleRNG rng;
		std::vector<std::vector<std::size_t>> reactionsDependanceGraph;
		std::vector<unsigned short int> trace;

		void ApplyBackward(const ECellEngine::Data::Reaction& _reaction);

		void ApplyForward(const ECellEngine::Data::Reaction& _reaction);

		void BuildDependancyGraph(const std::vector<ECellEngine::Data::Reaction>& _reactions);

		const float ComputeReactionPropensity(const unsigned int& _reactionIndex);

		void SolveBackward(const float& targetTime);

		void SolveForward(const float& targetTime);
	
	public:
		GillespieNRMRSolver(ECellEngine::Data::DataState& _dataState) :
			BiochemicalSolver(_dataState)
		{

		}

		virtual void Initialize(const ECellEngine::Data::Module&) override;

		virtual void Update(const ECellEngine::Data::Module& _module, const float& _deltaTime) override;
	};
}