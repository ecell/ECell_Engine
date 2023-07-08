#pragma once

#include "Core/Callback.hpp"
#include "Maths/Logic.hpp"

namespace ECellEngine::Maths
{
	/*!
	@brief Class used to execute a boolean operation.
	*/
	class LogicOperation
	{
	public:
		enum LogicalOperator
		{
			LogicalOperator_AND,
			LogicalOperator_OR,
			LogicalOperator_NOT
		};

	private:
		/*!
		@brief The logic operator used to compute the result.
		*/
		LogicalOperator operatorType = LogicalOperator_AND;

		/*!
		@brief The logic operator used to compute the result.
		*/
		const Logic* logic = nullptr;

		/*!
		@brief The left hand side operand.
		*/
		bool lhs = false;

		/*!
		@brief The right hand side operand.
		*/
		bool rhs = false;

		/*!
		@brief The result of the logic comparison at the previous
				evaluation.
		*/
		bool previousResult = false;

		/*!
		@brief The result of the logic comparison at the current
				evaluation.
		*/
		bool newResult = false;

	public:
		/*!
		@brief The placeholder for the subscription of ::UpdateLHS to a matching
				callback.
		*/
		std::shared_ptr<std::function<void(const bool, const bool)>> updateLHSSubToken = nullptr;

		/*!
		@brief The placeholder for the subscription of ::UpdateRHS to a matching
				callback.
		*/
		std::shared_ptr<std::function<void(const bool, const bool)>> updateRHSSubToken = nullptr;

		/*!
		@brief The callback whenever the value of ::lhs or ::rhs changes.
		*/
		ECellEngine::Core::Callback<bool, bool> onOperandChange;

		/*!
		@brief The callback whenever the value of ::newResult changes.
		*/
		ECellEngine::Core::Callback<bool, bool> onResultChange;

		LogicOperation() = default;

		/*!
		@brief Evaluates the logic comparison and returns the result.
		*/
		inline bool operator()() const noexcept
		{
			return (*logic)(lhs, rhs);
		}

		/*!
		@brief Returns the enum representation of the logical operator
				used in this operation.
		*/
		inline LogicalOperator& GetLogicalOperator() noexcept
		{
			return operatorType;
		}

		/*!
		@brief Sets the logical operator used to compute the result based on
				the value of ::operatorType.
		*/
		void SetLogic() noexcept;

		/*!
		@brief Updates the left hand side and triggers the callbacks
				::onOperandChange and ::onResultChange if the corresponding
				conditions are met.
		*/
		void UpdateLHS(const bool _previousValue, const bool _newValue) noexcept;


		/*!
		@brief Updates the right hand side and triggers the callbacks
				::onOperandChange and ::onResultChange if the corresponding
				conditions are met.
		*/
		void UpdateRHS(const bool _previousValue, const bool _newValue) noexcept;
	};
}