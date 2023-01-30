#pragma once

#include <cstddef>

#include "BiochemicalModule.hpp"
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
		std::unordered_multimap<std::string, std::string> reactionsDependanceGraph;//one reaction name maps to multiple reaction names
		std::vector<unsigned short int> trace;

		void ApplyBackward(const ECellEngine::Data::Reaction& _reaction);

		void ApplyForward(const ECellEngine::Data::Reaction& _reaction);

		void BuildDependancyGraph(const std::vector<std::string>& _reactions);

		const float ComputeReactionPropensity(std::string& _reactionName);

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