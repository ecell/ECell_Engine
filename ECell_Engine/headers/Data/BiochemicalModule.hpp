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

		/*!
		@brief A list of tokens that will keep the module informed of the destruction
				of reactions so that it can remove them from its local list of reactions.
		@see ::OnReactionDestroy, ECellEngine::Core::Callback
		*/
		std::vector<std::shared_ptr<std::function<void(Reaction*)>>> onReactionDestroySubTokens;

		/*!
		@brief A list of the IDs of the reactions that are part of this module.
		@details The IDs are the keys to the unordered map of reactions in the data state.
		@see ECellEngine::Data::DataState::reactions
		*/
		std::vector<std::size_t> reactions;

		/*!
		@brief A list of tokens that will keep the module informed of the destruction
				of parameters so that it can remove them from its local list of parameters.
		@see ::OnParameterDestroy, ECellEngine::Core::Callback
		*/
		std::vector<std::shared_ptr<std::function<void(Operand*)>>> onParameterDestroySubTokens;

		/*!
		@brief A list of the IDs of the parameters that are part of this module.
		@details The IDs are the keys to the unordered map of parameters in the data state.
		@see ECellEngine::Data::DataState::parameters
		*/
		std::vector<std::size_t> parameters;

		/*!
		@brief A list of tokens that will keep the module informed of the destruction
				of species so that it can remove them from its local list of species.
		@see ::OnSpeciesDestroy, ECellEngine::Core::Callback
		*/
		std::vector<std::shared_ptr<std::function<void(Operand*)>>> onSpeciesDestroySubTokens;

		/*!
		@brief A list of the IDs of the species that are part of this module.
		@details The IDs are the keys to the unordered map of species in the data state.
		@see ECellEngine::Data::DataState::species
		*/
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

		/*!
		@brief Returns the list of all the reactions IDs in the module.
		@details The intent is to gather the IDs of reactions that are part of
				 the module to further access them in the data state.
		@returns A reference to the list of reactions.
		@see ECellEngine::Data::DataState::reactions, ECellEngine::Data::DataState::GetReaction
		*/
		inline const std::vector<std::size_t>& GetAllReaction() const noexcept
		{
			return reactions;
		}

		/*!
		@brief Returns the list of all the parameters IDs in the module.
		@details The intent is to gather the IDs of parameters that are part of
				 the module to further access them in the data state.
		@returns A reference to the list of parameters.
		@see ECellEngine::Data::DataState::parameters, ECellEngine::Data::DataState::GetParameter
		*/
		inline const std::vector<std::size_t>& GetAllParameter () const noexcept
		{
			return parameters;
		}

		/*!
		@brief Returns the list of all the species IDs in the module.
		@details The intent is to gather the IDs of species that are part of
				 the module to further access them in the data state.
		@returns A reference to the list of species.
		@see ECellEngine::Data::DataState::species, ECellEngine::Data::DataState::GetSpecies
		*/
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

		/*!
		@brief Returns the ID of a reaction in the module stored in ::reactions.
		@param _idx The index of the reaction in ::reactions.
		@returns The ID of the reaction.
		*/
		inline const std::size_t GetReaction(const std::size_t _idx) const noexcept
		{
			return reactions[_idx];
		}

		/*!
		@brief Returns the ID of a parameter in the module stored in ::parameters.
		@param _idx The index of the parameter in ::parameters.
		@returns The ID of the parameter.
		*/
		inline const std::size_t GetParameter(const std::size_t _idx) const noexcept
		{
			return parameters[_idx];
		}

		/*!
		@brief Returns the ID of a species in the module stored in ::species.
		@param _idx The index of the species in ::species.
		@returns The ID of the species.
		*/
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

		/*!
		@brief Returns the name of the reaction with the given ID.
		@details This is a shortcut to the data state information.
		@param _id The ID of the reaction.
		@see ECellEngine::Data::DataState::reactions
		*/
		inline const char* GetReactionName(const std::size_t _id) const noexcept
		{
			return dataState.GetReaction(_id)->GetName();
		}

		/*!
		@brief Returns the name of the parameter with the given ID.
		@details This is a shortcut to the data state information.
		@param _id The ID of the parameter.
		@see ECellEngine::Data::DataState::parameters
		*/
		inline const char* GetParameterName(const std::size_t _id) const noexcept
		{
			return dataState.GetParameter(_id)->GetName();
		}

		/*!
		@brief Returns the name of the species with the given ID.
		@details This is a shortcut to the data state information.
		@param _id The ID of the species.
		@see ECellEngine::Data::DataState::species
		*/
		inline const char* GetSpeciesName(const std::size_t _id) const noexcept
		{
			return dataState.GetSpecies(_id)->GetName();
		}

		/*!
		@brief Adds an equation to the module.
		@details In effect the equation is added to the data state and the ID of the
				equation is stored in the local list ::equations of this module.
		@param _lhs The left hand side of the equation.
		@param _rhs The right hand side of the equation.
		*/
		void AddEquation(Operand* _lhs, Operation& _rhs);

		std::size_t AddReaction(const std::string _reactionName, const std::vector<std::size_t> _products,
			const std::vector<std::size_t> _reactants, const Operation _kineticLaw);

		/*!
		@brief Adds a parameter to the module.
		@details In effect the parameter is added to the data state and the ID of the
				parameter is stored in the local list ::parameters of this module.
		@param _parameterName The name of the parameter.
		@param _value The value of the parameter.
		@returns The ID of the parameter.
		*/
		std::size_t AddParameter(const std::string _parameterName, const float _value);

		/*!
		@brief Adds a species to the module.
		@details In effect the species is added to the data state and the ID of the
				species is stored in the local list ::species of this module.
		@param _speciesName The name of the species.
		@param _quantity The quantity of the species.
		@returns The ID of the species.
		*/
		std::size_t AddSpecies(const std::string _speciesName, const float _quantity);

		/*!
		@brief Checks if the solver is a valid solver for this module.
		@details The solver must be a BiochemicalSolver. Performs a dynamic cast
				to check if the solver is a BiochemicalSolver.
		@param _solver The solver to be checked.
		@returns True if the solver is a BiochemicalSolver, false otherwise.
		*/
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
		@brief Callback function to be called when a parameter is destroyed in the
				data state so that we can remove it from the local list of parameters
				of this module.
		@param _op The pointer to the operand base class information of the parameter
					being destroyed.
		@see onParameterDestroySubTokens.
		*/
		void OnParameterDestroy(Operand* _op) noexcept;

		/*!
		@brief Callback function to be called when a reaction is destroyed in the
				data state so that we can remove it from the local list of reactions
				of this module.
		@param _react The pointer to the reaction that is being destroyed.
		@see onReactionDestroySubTokens.
		*/
		void OnReactionDestroy(Reaction* _react) noexcept;

		/*!
		@brief Callback function to be called when a species is destroyed in the
				data state so that we can remove it from the local list of species
				of this module.
		@param _op The pointer to the operand base class information of the species
					being destroyed.
		@see onSpeciesDestroySubTokens.
		*/
		void OnSpeciesDestroy(Operand* _op) noexcept;

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