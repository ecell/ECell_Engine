#pragma once

#include <vector>

#include "Solver/BiochemicalSolver.hpp"
#include "Data/Module.hpp"

namespace ECellEngine::Data
{
	class BiochemicalModule final : public Module
	{
	protected:
		std::vector<std::string> equations;
		std::vector<std::string> reactions;
		std::vector<std::string> parameters;
		std::vector<std::string> species;

	public:
		BiochemicalModule(DataState& _dataState, char* _name = "NewBioChemicalAsset") :
			Module(_dataState, _name)
		{

		}

		inline const std::vector<std::string>& GetAllEquation() const noexcept
		{
			return equations;
		}

		inline const std::vector<std::string>& GetAllReaction() const noexcept
		{
			return reactions;
		}

		inline const std::vector<std::string>& GetAllParameter () const noexcept
		{
			return parameters;
		}

		inline const std::vector<std::string>& GetAllSpecies() const noexcept
		{
			return species;
		}
		
		inline const std::string& GetEquation(const std::size_t& _idx) const noexcept
		{
			return equations[_idx];
		}

		inline const std::string& GetReaction(const std::size_t& _idx) const noexcept
		{
			return reactions[_idx];
		}

		inline const std::string& GetParameter(const std::size_t& _idx) const noexcept
		{
			return parameters[_idx];
		}

		inline const std::string& GetSpecies(const std::size_t& _idx) const noexcept
		{
			return species[_idx];
		}

		void AddEquation(Operand* _lhs, Operation& _rhs);

		void AddReaction(const std::string _reactionName,
			const std::vector<std::string> _products,
			const std::vector<std::string> _reactants,
			const Operation _kineticLaw);

		void AddParameter(const std::string _parameterName, const float _value);

		void AddSpecies(const std::string _speciesName, const float _quantity);

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