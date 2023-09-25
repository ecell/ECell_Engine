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
	private:

		/*!
		@brief The ID of the operation.
		*/
		std::size_t id = 0;

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
		ECellEngine::Core::Callback<const bool, const bool> onOperandChange;

		/*!
		@brief The callback whenever the value of ::newResult changes.
		*/
		ECellEngine::Core::Callback<const bool, const bool> onResultChange;

		LogicOperation(const std::size_t _id) :
			id(_id)
		{
			SetLogic();//update the logic operator based on the value of ::operatorType
		}

		/*!
		@brief Operator used to compare the ID of this operation with the ID of
				another operation.
		*/
		inline bool operator<(const LogicOperation& _rhs) const noexcept
		{
			return id < _rhs.id;
		}

		/*!
		@brief Operator used to compare the ID of this operation with a given
				ID.
		*/
		inline bool operator<(const std::size_t _id) const noexcept
		{
			return id < _id;
		}

		/*!
		@brief Evaluates the logic comparison and returns the result.
		*/
		inline bool operator()() const noexcept
		{
			return (*logic)(lhs, rhs);
		}

		/*!
		@brief Gets the ID of this operation.
		*/
		inline std::size_t GetID() const noexcept
		{
			return id;
		}

		/*!
		@brief Returns the reference to the enum representation of the logical
				operator used in this operation.
		*/
		inline LogicalOperator& GetLogicalOperator() noexcept
		{
			return operatorType;
		}

		/*!
		@brief The value of the left hand side operand.
		*/
		inline bool GetLHS() const noexcept
		{
			return lhs;
		}

		/*!
		@brief The value of the right hand side operand.
		*/
		inline bool GetRHS() const noexcept
		{
			return rhs;
		}

		/*!
		@brief Sets the logical operator used to compute the result based on
				the value of ::operatorType.
		@remark There is no setter for ::operatorType but it can be changed
				through the reference returned by ::GetLogicalOperator.
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