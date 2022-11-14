#pragma once

#include <cstddef>

#include "BiochemicalSolver.hpp"
#include "IndexedMinHeap.hpp"
#include "Reaction.hpp"
#include "ReversibleRNG.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Solvers
{
	class GillespieNRMRSolver : BiochemicalSolver
	{
	private:
		
		IndexedMinHeap tauIMH;
		ReversibleRNG rng;
		std::vector<std::vector<std::size_t>> reactionsDependanceGraph;
		std::vector<unsigned short int> trace;


		void ApplyBackward(const Reaction& _reaction);

		void ApplyForward(const Reaction& _reaction);

		void BuildDependancyGraph(const std::vector<Reaction>& _reactions);

		const float ComputeReactionPropensity(const unsigned int& _reactionIndex);

		void SolveBackward(const float& targetTime);

		void SolveForward(const float& targetTime);
	
	public:
		GillespieNRMRSolver(DataState* _dataState) :
			BiochemicalSolver(_dataState)
		{

		}

		virtual void Initialize(const Module&) override;

		virtual void Update(const Module& _module, const float& _deltaTime) override;
	};
}