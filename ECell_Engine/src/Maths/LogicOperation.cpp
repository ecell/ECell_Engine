#include "Maths/LogicOperation.hpp"

void ECellEngine::Maths::LogicOperation::SetLogic() noexcept
{
	switch (operatorType)
	{
	case(LogicalOperator_AND):
		logic = &logics.and;
		break;
	case(LogicalOperator_OR):
		logic = &logics.or;
		break;
	case(LogicalOperator_NOT):
		logic = &logics.not;
		break;
	default:
		break;
	}
}

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