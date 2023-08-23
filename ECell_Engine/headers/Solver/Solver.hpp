#pragma once

/*!
@file Solver.hpp
@brief The base class for all solvers.
*/

#include "Data/DataState.hpp"
#include "Data/Module.hpp"
#include "Solver/Stepper.hpp"

//Forward declaration
namespace ECellEngine::Data
{
	class Module;
}

namespace ECellEngine::Solvers
{
	/*!
	@brief The base class for all solvers.
	*/
	class Solver
	{
	private:
		/*!
		@brief The name of the solver.
		*/
		std::string name;

	protected:
		ECellEngine::Data::DataState& dataState;

	public:

		/*!
		@brief The comparison function object for the solver's id.
		*/
		struct CompareID
		{
			/*!
			@brief Compares if solver @p _s1 id is less (strictly) than solver
					@p _s2 id.
			@param _s1 The left hand side of the comparison.
			@param _s2 The right hand side of the comparison.
			@returns True if @p _s1 < @p _s2 id, false otherwise.
			*/
			inline bool operator()(const Solver& _s1, const Solver& _s2) const
			{
				return _s1.id < _s2.id;
			}

			/*!
			@brief Compares if solver @p _s1 id is less (strictly) than solver
					@p _s2 id.
			@param _s1 The left hand side of the comparison.
			@param _id The right hand side of the comparison.
			@returns True if @p _s1 < @p _id, false otherwise.
			*/
			inline bool operator()(const Solver& _s1, const std::size_t _id)
			{
				return _s1.id < _id;
			}
			/*!
			@brief Compares if solver @p _s1 id is less (strictly) than solver
					@p _s2 id.
			@param _s1 The left hand side of the comparison.
			@param _s2 The right hand side of the comparison.
			@returns True if @p _s1 < @p _s2 id, false otherwise.
			*/
			inline bool operator()(std::shared_ptr<Solver> _s1, std::shared_ptr<Solver> _s2)
			{
				return _s1->id < _s2->id;
			}

			/*!
			@brief Compares if solver @p _s1 id is less (strictly) than solver
					@p _s2 id.
			@param _s1 The left hand side of the comparison.
			@param _id The right hand side of the comparison.
			@returns True if @p _s1 < @p _id, false otherwise.
			*/
			inline bool operator()(std::shared_ptr<Solver> _s1, const std::size_t _id)
			{
				return _s1->id < _id;
			}
		};

		/*!
		@brief The unique id of the solver.
		*/
		const std::size_t id;

		Solver(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			dataState{ _dataState }, id{ ++_dataState.idProvider }, name{_name}
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
		@brief API to execute code once before the solver's update loop.
		*/
		virtual void Start() = 0;

		/*!
		@brief Runs the solver until the given time described by the timer is
				reached.
		@param _timer The timer containing information about the simulation's
				elapsed time and delta Time.
		*/
		virtual void Update(const ECellEngine::Core::Timer& _timer) = 0;
	};
}