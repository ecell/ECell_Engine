#pragma once

namespace ECellEngine::Data
{
	class Module;
}

#include "Data/DataState.hpp"
#include "Data/Module.hpp"
#include "Solver/Stepper.hpp"

namespace ECellEngine::Solvers
{
	class Solver
	{
	private:
		std::string name;

	protected:
		ECellEngine::Data::DataState& dataState;

	public:
		Solver(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			dataState{ _dataState }, name{_name}
		{

		}

		virtual ~Solver() = default;

		/*!
		@brief Gets the name of the solver.
		@returns ::name.
		*/
		inline std::string& GetName() noexcept
		{
			return name;
		}

		/*!
		@brief Gets the stepper used by the derived solver.
		@details The declaration of the stepper is in the derived solver.
		*/
		virtual Stepper* GetStepper() noexcept = 0;

		/*!
		@brief Sets the name of the solver (::name).
		*/
		inline void SetName(std::string& _name) noexcept
		{
			name = _name;
		}

		/*!
		@brief Initializes the solver based on the given module.
		@param _module The module to initialize the solver with.
		*/
		virtual void Initialize(const ECellEngine::Data::Module*) = 0;

		/*!
		@brief Runs the solver until the given time described by the timer is
				reached.
		@param _timer The timer containing information about the simulation's
				elapsed time and delta Time.
		*/
		virtual void Update(const ECellEngine::Core::Timer& _timer) = 0;
	};
}