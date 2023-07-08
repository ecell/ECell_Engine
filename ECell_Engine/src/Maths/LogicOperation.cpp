#include "Maths/LogicOperation.hpp"

void ECellEngine::Maths::LogicOperation::UpdateLHS(const bool _previousValue, const bool _newValue) noexcept
{	
	bool newRes = (*logic)(_newValue, rhs);

	if (newResult != newRes)
	{
		onResultChange(newResult, newRes);
	}

	if (_previousValue != _newValue)
	{
		lhs = _newValue;
		onOperandChange(newResult, newRes);
	}

	previousResult = newResult;
	newResult = newRes;
}

void ECellEngine::Maths::LogicOperation::UpdateRHS(const bool _previousValue, const bool _newValue) noexcept
{
	bool newRes = (*logic)(lhs, _newValue);

	if (newResult != newRes)
	{
		onResultChange(newResult, newRes);
	}

	if (_previousValue != _newValue)
	{
		rhs = _newValue;
		onOperandChange(newResult, newRes);
	}

	previousResult = newResult;
	newResult = newRes;
}