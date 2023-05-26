#pragma once

#include "Maths/Operation.hpp"

namespace ECellEngine::Maths
{
	class Equation
	{
	private:
		Operand* lhs;
		Operation rhs;

	public:
		std::string name;

		Equation(const std::string _name, Operand* _lhs, Operation _rhs) :
			name{ _name }, lhs{ _lhs }, rhs{ _rhs }
		{

		}

		/*!
		@brief Returns the operation (right hand side) of the equation.
		*/
		inline Operation& GetOperation() noexcept const
		{
			return rhs;
		}

		/*
		@brief Returns the operand (left hand side) of the equation.
		@details This is the operand that is being assigned to.
		*/
		inline const Operand* GetOperand() noexcept const
		{
			return lhs;
		}

		/*
		@brief Returns the value of the operand (left hand side) of the equation.
		*/
		inline const float Get() noexcept const
		{
			return lhs->Get();
		}

		/*
		@brief Computes the equation, assigns the result to the operand	(left
				hand side) and returns the result.
		*/
		inline float Compute() noexcept
		{
			lhs->Set(rhs.Get());
			return lhs->Get();
		}
	};
}