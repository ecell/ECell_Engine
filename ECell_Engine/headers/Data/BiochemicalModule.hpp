#pragma once

#include <vector>

#include "Core/Callback.hpp"
#include "Solver/BiochemicalSolver.hpp"
#include "Data/Module.hpp"

namespace ECellEngine::Data
{
	class BiochemicalModule final : public Module
	{
	protected:

		/*!
		@brief A list of tokens that will keep the module informed of the destruction
				of equations so that it can remove them from its local list of equations.
		@see ::OnEquationDestroy, ECellEngine::Core::Callback
		*/
		std::vector<std::shared_ptr<std::function<void(Equation*)>>> onEquationDestroySubTokens;

		/*!
		@brief A list of the IDs of the equations that are part of this module.
		@details The IDs are the keys to the unordered map of equations in the data state.
		@see ECellEngine::Data::DataState::equations
		*/
		std::vector<std::size_t> equations;
		std::vector<std::size_t> reactions;
		std::vector<std::size_t> parameters;
		std::vector<std::size_t> species;

	public:
		BiochemicalModule(DataState& _dataState, char* _name = "NewBioChemicalAsset") :
			Module(_dataState, _name)
		{

		}

		/*!
		@brief Returns the list of all the equations IDs in the module.
		@details The intent is to gather the IDs of equations that are part of
				 the module to further access them in the data state.
		@returns A reference to the list of equations.
		@see ECellEngine::Data::DataState::equations, ECellEngine::Data::DataState::GetEquation
		*/
		inline const std::vector<std::size_t>& GetAllEquation() const noexcept
		{
			return equations;
		}

		inline const std::vector<std::size_t>& GetAllReaction() const noexcept
		{
			return reactions;
		}

		inline const std::vector<std::size_t>& GetAllParameter () const noexcept
		{
			return parameters;
		}

		inline const std::vector<std::size_t>& GetAllSpecies() const noexcept
		{
			return species;
		}
		
		/*!
		@brief Returns the ID of an equation in the module stored in ::equations.
		@param _idx The index of the equation in ::equations.
		@returns The ID of the equation.
		*/
		inline const std::size_t GetEquation(const std::size_t _idx) const noexcept
		{
			return equations[_idx];
		}

		inline const std::size_t GetReaction(const std::size_t _idx) const noexcept
		{
			return reactions[_idx];
		}

		inline const std::size_t GetParameter(const std::size_t _idx) const noexcept
		{
			return parameters[_idx];
		}

		inline const std::size_t GetSpecies(const std::size_t _idx) const noexcept
		{
			return species[_idx];
		}

		/*!
		@brief Returns the name of the equation with the given ID.
		@details This is a shortcut to the data state information.
		@param _id The ID of the equation.
		@see ECellEngine::Data::DataState::equations
		*/
		inline const char* GetEquationName(const std::size_t _id) const noexcept
		{
			return dataState.GetEquation(_id)->GetName();
		}

		inline const char* GetReactionName(const std::size_t _id) const noexcept
		{
			return dataState.GetReaction(_id)->GetName();
		}

		inline const char* GetParameterName(const std::size_t _id) const noexcept
		{
			return dataState.GetParameter(_id)->GetName();
		}

		inline const char* GetSpeciesName(const std::size_t _id) const noexcept
		{
			return dataState.GetSpecies(_id)->GetName();
		}

		/*!
		@brief Adds an equation to the module.
		@details In effect the equation is added to the data state and the ID of the
				equation is stored in the local list of equations of this module.
		@param _lhs The left hand side of the equation.
		@param _rhs The right hand side of the equation.
		*/
		void AddEquation(Operand* _lhs, Operation& _rhs);

		std::size_t AddReaction(const std::string _reactionName, const std::vector<std::size_t> _products,
			const std::vector<std::size_t> _reactants, const Operation _kineticLaw);

		std::size_t AddParameter(const std::string _parameterName, const float _value);

		std::size_t AddSpecies(const std::string _speciesName, const float _quantity);

		bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept override;

		/*!
		@brief Callback function to be called when an equation is destroyed in the
				data state so that we can remove it from the local list of equations
				of this module.
		@param _eq The pointer to the equation that is being destroyed.
		@see onEquationDestroySubTokens.
		*/
		void OnEquationDestroy(Equation* _eq) noexcept;

		/*!
		@brief Resets the species, parameters, equations and reaction kinetic
				law's cache values(in this order).
		@remark It is likely that species and parameters will be reset several
				times by also reseting the equations. It's a waste of resources,
				but it's the only way to ensure that species or parameters that
				are not part of any equation are reset without building an
				additional data structure to keep track of them. We will see if
				it becomes problem it in the future.
		*/
		void Reset() noexcept override;
	};
}