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
	previousResult = newResult;
	newResult = (*logic)(_newValue, rhs);

	if (previousResult != newResult)
	{
		onResultChange(previousResult, newResult);
	}

	if (_previousValue != _newValue)
	{
		lhs = _newValue;
		onOperandChange(previousResult, newResult);
	}
}

void ECellEngine::Maths::LogicOperation::UpdateRHS(const bool _previousValue, const bool _newValue) noexcept
{
	previousResult = newResult;
	newResult = (*logic)(lhs, _newValue);

	if (previousResult != newResult)
	{
		onResultChange(previousResult, newResult);
	}

	if (_previousValue != _newValue)
	{
		rhs = _newValue;
		onOperandChange(previousResult, newResult);
	}
}