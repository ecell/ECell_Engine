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
		@brief Sets the logical operator used to compute the result.
		*/
		inline void SetLogic(const Logic* _logic) noexcept
		{
			logic = _logic;
		}

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