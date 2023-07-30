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

		Equation(Operand* _lhs, Operation& _rhs) :
			lhs{ _lhs }, rhs{ _rhs }
		{

		}

		/*
		@brief Returns the value of the operand (left hand side) of the equation.
		*/
		inline const float Get() const noexcept
		{
			return lhs->Get();
		}

		/*!
		@brief Returns the name of the operand (left hand side) of the equation.
		*/
		inline const std::string GetName() const noexcept
		{
			return lhs->name;
		}

		/*
		@brief Returns the operand (left hand side) of the equation.
		@details This is the operand that is being assigned to.
		*/
		inline Operand* GetOperand() noexcept
		{
			return lhs;
		}

		/*!
		@brief Returns the operation (right hand side) of the equation.
		*/
		inline Operation& GetOperation() noexcept
		{
			return rhs;
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