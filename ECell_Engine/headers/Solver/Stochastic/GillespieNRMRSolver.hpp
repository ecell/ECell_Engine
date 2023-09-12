#pragma once

#include <cstddef>

#include "Data/IndexedMinHeap.hpp"
#include "Data/Reaction.hpp"
#include "Maths/ReversibleRNG.hpp"
#include "Solver/BiochemicalSolver.hpp"
#include "Solver/Stochastic/GillespieNRMRStepper.hpp"

namespace ECellEngine::Solvers::Stochastic
{
	class GillespieNRMRSolver final : public BiochemicalSolver
	{
	private:

		GillespieNRMRStepper stepper;

		ECellEngine::Data::IndexedMinHeap tauIMH;
		ReversibleRNG rng;
		std::unordered_multimap<std::size_t, std::size_t> reactionsDependanceGraph;//one reaction idx maps to multiple reaction idx
		std::vector<unsigned short int> trace;

		void ApplyBackward(const std::string& _reactionName);

		void ApplyForward(const std::string& _reactionName);

		void BuildDependancyGraph(const std::vector<std::size_t>& _reactions);

		const float ComputeReactionPropensity(const std::size_t _reactionID);

		void SolveBackward(const float& targetTime);

		void SolveForward(const float& targetTime);
	
	public:
		GillespieNRMRSolver(ECellEngine::Data::DataState& _dataState, const char* _name) :
			BiochemicalSolver(_dataState, _name)
		{
			
		}

		inline Stepper* GetStepper() noexcept override
		{
			return &stepper;
		}

		void Clear() override;

		void Initialize(const ECellEngine::Data::Module*) override;

		void Reset() override;

		void Start() override;

		void Update(const ECellEngine::Core::Timer& _timer) override;
	};
}