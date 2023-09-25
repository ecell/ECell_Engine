#pragma once

/*!
@file DataState.hpp
@brief Defines all the data used in a simulation space.
*/

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Core/Events/ModifyDataStateValueEvent.hpp"
#include "Core/IDProvider.hpp"
#include "Core/Trigger.hpp"
#include "Data/Reaction.hpp"
#include "Data/Parameter.hpp"
#include "Data/Species.hpp"
#include "Maths/Equation.hpp"
#include "Maths/LogicOperation.hpp"

namespace ECellEngine::Data
{
	struct DataState
	{
	private:

		/*!
		@brief A map of parameter names to their IDs.
		@remark TEMPORARY
		@remark Only to be used while transitioning from name-as-ID to ID-as-ID.
		*/
		std::unordered_map<std::string, std::size_t> paramNamesToIds;

		/*!
		@brief A map of parameter names to their IDs.
		@remark TEMPORARY
		@remark Only to be used while transitioning from name-as-ID to ID-as-ID.
		*/
		std::unordered_map<std::string, std::size_t> speciesNamesToIds;

		/*!
		@brief A map of reaction names to their IDs.
		@remark TEMPORARY
		@remark Only to be used while transitioning from name-as-ID to ID-as-ID.
		*/
		std::unordered_map<std::string, std::size_t> reactionNamesToIds;


		std::unordered_map<std::size_t, std::shared_ptr<Maths::Equation>> equations;
		std::unordered_map<std::size_t, std::shared_ptr<Reaction>> reactions;
		std::unordered_map<std::size_t, std::shared_ptr<Parameter>> parameters;
		std::unordered_map<std::size_t, std::shared_ptr<Species>> species;

		std::unordered_multimap<std::size_t, std::size_t> operandsToOperations;

		std::vector<std::shared_ptr<Maths::Operation>> operations;
		std::vector<std::shared_ptr<Maths::LogicOperation>> logicOperations;
		std::vector<std::shared_ptr<Core::Events::ModifyDataStateValueEvent>> modifyDataStateValueEvents;
		std::vector<std::shared_ptr<Core::Trigger<Operand*, Operand*>>> triggers;

	public:
		Core::IDProvider idProvider;

		DataState()
		{

		}

		inline std::shared_ptr<Maths::Equation> GetEquation(const std::size_t _lhsID) const
		{
			return equations.at(_lhsID);
		}

		inline const std::unordered_map<std::size_t, std::shared_ptr<Maths::Equation>>& GetEquations() const
		{
			return equations;
		}

		Operand* GetOperand(const std::size_t _id);

		inline std::unordered_multimap<std::size_t, std::size_t>& GetOperandsToOperations()
		{
			return operandsToOperations;
		}

		/*!
		@brief Gets a reaction by its name.
		@details Slow as it needs to translate the name string to the ID to search in ::reactions.
				 Try using ::GetReaction(const std::size_t _reactionID) instead. Or, at least,
				 try not to use this function in a loop.
		@param _reactionName The name of the reaction to get.
		@remark You must use catch on std::out_of_range yourself to check if
				the reaction exists.
		@returns A shared pointer to the reaction.
		*/
		inline std::shared_ptr<Reaction> GetReaction(const std::string& _reactionName) const
		{
			return reactions.at(reactionNamesToIds.at(_reactionName));
		}

		/*!
		@brief Gets a reaction by its ID.
		@details Searches in the unordered map ::reactions.
		@param _reactionID The ID of the reaction to get.
		@remark You must use catch on std::out_of_range yourself to check if
				the reaction exists.
		@returns A shared pointer to the reaction.
		*/
		inline std::shared_ptr<Reaction> GetReaction(const std::size_t _reactionID) const
		{
			return reactions.at(_reactionID);
		}

		/*!
		@brief Gets all reactions.
		@returns A const reference to the unordered map ::reactions.
		*/
		inline const std::unordered_map<std::size_t, std::shared_ptr<Reaction>>& GetReactions() const
		{
			return reactions;
		}

		/*!
		@brief Gets a parameter by its name.
		@details Slow as it needs to translate the name string to the ID to search in ::parameters.
				 Try using ::GetParameter(const std::size_t _parameterID) instead. Or, at least,
				 try not to use this function in a loop.
		@param _parameterName The name of the parameter to get.
		@remark You must use catch on std::out_of_range yourself to check if
				the parameter exists.
		@returns A shared pointer to the parameter.
		*/
		inline std::shared_ptr<Parameter> GetParameter(const std::string& _parameterName) const
		{
			return parameters.at(paramNamesToIds.at(_parameterName));
		}

		/*!
		@brief Gets a parameter by its ID.
		@details Searches in the unordered map ::parameters.
		@param _parameterID The ID of the parameter to get.
		@remark You must use catch on std::out_of_range yourself to check if
				the parameter exists.
		@returns A shared pointer to the parameter.
		*/
		inline std::shared_ptr<Parameter> GetParameter(const std::size_t _parameterID) const
		{
			return parameters.at(_parameterID);
		}

		/*!
		@brief Gets all parameters.
		@returns A const reference to the unordered map ::parameters.
		*/
		inline const std::unordered_map<std::size_t, std::shared_ptr<Parameter>>& GetParameters() const
		{
			return parameters;
		}

		/*!
		@brief Gets a species by its name.
		@details Slow as it needs to translate the name string to the ID to search in ::species.
				 Try using ::GetSpecies(const std::size_t _speciesID) instead. Or, at least,
				 try not to use this function in a loop.
		@param _speciesName The name of the species to get.
		@remark You must use catch on std::out_of_range yourself to check if
				the species exists.
		@returns A shared pointer to the species.
		*/
		inline std::shared_ptr<Species> GetSpecies(const std::string& _speciesName) const
		{
			return species.at(speciesNamesToIds.at(_speciesName));
		}

		/*!
		@brief Gets a species by its ID.
		@details Searches in the unordered map ::species.
		@param _speciesID The ID of the species to get.
		@remark You must use catch on std::out_of_range yourself to check if
				the species exists.
		@returns A shared pointer to the species.
		*/
		inline std::shared_ptr<Species> GetSpecies(const std::size_t _speciesID) const
		{
			return species.at(_speciesID);
		}

		/*!
		@brief Gets all species.
		@returns A const reference to the unordered map ::species.
		*/
		inline const std::unordered_map<std::size_t, std::shared_ptr<Species>>& GetAllSpecies() const
		{
			return species;
		}

		inline const std::vector<std::shared_ptr<Maths::Operation>>& GetOperations() const
		{
			return operations;
		}

		inline const std::vector<std::shared_ptr<Maths::LogicOperation>>& GetLogicOperations() const
		{
			return logicOperations;
		}

		inline const std::vector<std::shared_ptr<Core::Events::ModifyDataStateValueEvent>>& GetModifyDataStateValueEvents() const
		{
			return modifyDataStateValueEvents;
		}

		inline const std::vector<std::shared_ptr<Core::Trigger<Operand*, Operand*>>>& GetTriggers() const
		{
			return triggers;
		}

		inline std::shared_ptr<Reaction> AddReaction(const char* _reactionName,
			const std::vector<std::size_t> _products,
			const std::vector<std::size_t> _reactants,
			const Operation _kineticLaw)
		{
			++idProvider;
			return reactions.emplace(idProvider(), std::make_shared<Reaction>(_reactionName, idProvider(), _products, _reactants, _kineticLaw)).first->second;
		}

		inline std::shared_ptr<Maths::Equation> AddEquation(Operand* _lhs, Operation& _rhs)
		{
			return equations.emplace(_lhs->GetID(), std::make_shared<Maths::Equation>(_lhs, _rhs)).first->second;
		}

		inline std::shared_ptr<Core::Events::ModifyDataStateValueEvent> AddModifyDataStateValueEvent()
		{
			return modifyDataStateValueEvents.emplace_back(std::make_shared<Core::Events::ModifyDataStateValueEvent>(++idProvider));
		}

		inline std::shared_ptr<Parameter> AddParameter(const char* _parameterName, const float _value)
		{
			++idProvider;
			return parameters.emplace(idProvider(), std::make_shared<Parameter>(_parameterName, idProvider(), _value)).first->second;
		}
		
		inline std::shared_ptr<Species> AddSpecies(const char* _speciesName, const float _quantity)
		{
			++idProvider;
			return species.emplace(idProvider(), std::make_shared<Species>(_speciesName, idProvider(), _quantity)).first->second;
		}

		inline std::shared_ptr<Maths::Operation> AddOperation()
		{
			return operations.emplace_back(std::make_shared<Maths::Operation>(++idProvider));
		}

		inline std::shared_ptr<Maths::LogicOperation> AddLogicOperation()
		{
			return logicOperations.emplace_back(std::make_shared<Maths::LogicOperation>(++idProvider));
		}

		inline std::shared_ptr<Core::Trigger<Operand*, Operand*>> AddTrigger() noexcept
		{
			return triggers.emplace_back(std::make_shared<ECellEngine::Core::Trigger<Operand*, Operand*>>(++idProvider));
		}

		/*!
		@brief Clears all data.
		@details Includes ::equations, ::parameters, ::reactions, ::species, ::operations,
					::operandsToOperations ::logicOperations, ::modifyDataStateValueEvents,
					and ::triggers.
		*/
		void Clear() noexcept;

		/*!
		@brief Erases all data of a certain type in this data state.
		@details Valid values for @p _dataType are: Equation, Parameter, Reaction,
					Species, OperandsToOperation, Arithmetic, Logic,
					ModifyDataStateValueEvent, Trigger.
		*/
		bool EraseAllDataOfType(const char* _dataType) noexcept;

		inline void LinkOperandToOperation(const std::size_t _operandID, const std::size_t _operationID)
		{
			operandsToOperations.emplace(_operandID, _operationID);
		}

		/*!
		@brief Resets the data that have an initial value.
		@details Includes species, parameters and equations. Then, reactions'
				 kinetic laws are re-evaluated.
		*/
		void Reset() noexcept;
	};
}