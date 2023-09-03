#pragma once

#include <vector>

#include "Solver/BiochemicalSolver.hpp"
#include "Data/Module.hpp"

namespace ECellEngine::Data
{
	class BiochemicalModule final : public Module
	{
	protected:
		std::vector<std::size_t> equations;
		std::vector<std::size_t> reactions;
		std::vector<std::size_t> parameters;
		std::vector<std::size_t> species;

	public:
		BiochemicalModule(DataState& _dataState, char* _name = "NewBioChemicalAsset") :
			Module(_dataState, _name)
		{

		}

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

		inline const std::string& GetEquationName(const std::size_t _idx) const noexcept
		{
			return dataState.GetEquation(_idx)->GetName();
		}

		inline const std::string& GetReactionName(const std::size_t _idx) const noexcept
		{
			return dataState.GetReaction(_idx)->name;
		}

		inline const std::string& GetParameterName(const std::size_t _idx) const noexcept
		{
			return dataState.GetParameter(_idx)->name;
		}

		inline const std::string& GetSpeciesName(const std::size_t _idx) const noexcept
		{
			return dataState.GetSpecies(_idx)->name;
		}

		void AddEquation(Operand* _lhs, Operation& _rhs);

		std::size_t AddReaction(const std::string _reactionName, const std::vector<std::size_t> _products,
			const std::vector<std::size_t> _reactants, const Operation _kineticLaw);

		std::size_t AddParameter(const std::string _parameterName, const float _value);

		std::size_t AddSpecies(const std::string _speciesName, const float _quantity);

		bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept override;

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