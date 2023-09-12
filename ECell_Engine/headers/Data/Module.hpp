#pragma once

/*!
@file Module.hpp
@brief The base class for all modules.
*/

#include "Data/DataState.hpp"
#include "Logging/Logger.hpp"
#include "Solver/Solver.hpp"

//Forward declaration
namespace ECellEngine::Solvers
{
	class Solver;
}

namespace ECellEngine::Data
{
	/*!
	@brief The base class for all modules.
	*/
	class Module
	{
	private:
		/*!
		@brief The name of the module.
		*/
		char name[64];

	protected:
		/*!
		@brief The data state this module is related to.
		*/
		DataState& dataState;

	public:

		/*!
		@brief The comparison function object for modules' ids.
		*/
		struct CompareID
		{
			/*!
			@brief Compares if module @p _m1 id is less (strictly) than module
					@p _m2 id.
			@param _m1 The left hand side of the comparison.
			@param _m2 The right hand side of the comparison.
			@returns True if @p _m1 < @p _m2 id; false otherwise.
			*/
			inline bool operator()(const Module& _m1, const Module& _m2)
			{
				return _m1.id < _m2.id;
			}

			/*!
			@brief Compares if module @p _m1 id is less (strictly) than module
					@p _m2 id.
			@param _m1 The left hand side of the comparison.
			@param _id The right hand side of the comparison.
			@returns True if @p _m1 < @p _id; false otherwise.
			*/
			inline bool operator()(const Module& _m1, const std::size_t _id)
			{
				return _m1.id < _id;
			}

			/*!
			@brief Compares if module @p _m1 id is less (strictly) than module
					@p _m2 id.
			@param _m1 The left hand side of the comparison.
			@param _m2 The right hand side of the comparison.
			@returns True if @p _m1 < @p _m2 id; false otherwise.
			*/
			inline bool operator()(std::shared_ptr<Module> _m1, std::shared_ptr<Module> _m2)
			{
				return _m1->id < _m2->id;
			}

			/*!
			@brief Compares if module @p _m1 id is less (strictly) than module
					@p _m2 id.
			@param _m1 The left hand side of the comparison.
			@param _id The right hand side of the comparison.
			@returns True if @p _m1 < @p _id; false otherwise.
			*/
			inline bool operator()(std::shared_ptr<Module> _m1, const std::size_t _id)
			{
				return _m1->id < _id;
			}
		};

		/*!
		@brief The unique ID of the module.
		*/
		const std::size_t id;
		
		Module(DataState& _dataState, const char* _name) :
			dataState{_dataState}, id{ ++_dataState.idProvider }
		{
			Util::StrCopy(name, _name, sizeof(name));
		}

		/*!
		@brief Gets the name of the module.
		@return ::name
		*/
		inline char* GetName() noexcept
		{
			return name;
		}

		/*!
		@brief Sets the name of the module.
		*/
		inline void SetName(const char* _name) noexcept
		{
			Util::StrCopy(name, _name, sizeof(name));
		}

		/*!
		@brief Checks whether a given solver is valid for this module.
		@details For example, a derived module may only accept a specific solver type.
				 Verification is done by trying to dynamic_cast the solver to the desired type
				 for each derived module.
		@param _solver The solver to check.
		*/
		virtual bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept = 0;

		/*!
		@brief Resets the module to an initial state.
		*/
		virtual void Reset() noexcept = 0;

	};
}