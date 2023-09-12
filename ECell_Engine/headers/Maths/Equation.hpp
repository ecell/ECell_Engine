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
		inline char* GetName() noexcept
		{
			return lhs->GetName();
		}

		/*!
		@brief Returns the ID of the operand (left hand side) of the equation.
		*/
		inline const std::size_t GetID() const noexcept
		{
			return lhs->GetID();
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

		/*!
		@brief Sets the name of the operand (left hand side) of the equation.
		@param _name The new name of the operand.
		*/
		inline void SetName(const char* _name) noexcept
		{
			lhs->SetName(_name);
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

		/*!
		@brief Resets the right hand side of the equation and assigns the result
				to the operand (left hand side).
		*/
		inline void Reset() noexcept
		{
			lhs->Set(rhs.Reset());
		}
	};
}