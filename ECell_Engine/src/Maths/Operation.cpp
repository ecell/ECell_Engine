#include <cassert>
#include "Maths/Operation.hpp"

void ECellEngine::Maths::Operation::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedSpecies(out_involvedSpecies, clearOutVector);

	for (Operand* operand : operands)
	{
		operand->GetInvolvedSpecies(out_involvedSpecies, false);
	}
}

void ECellEngine::Maths::Operation::GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedParameters(out_involvedParameters, clearOutVector);

	for (Operand* operand : operands)
	{
		operand->GetInvolvedParameters(out_involvedParameters, false);
	}
}

void ECellEngine::Maths::Operation::GetInvolvedEquations(std::vector<std::string>& out_involvedEquations, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedEquations(out_involvedEquations, clearOutVector);

	for (Operand* operand : operands)
	{
		operand->GetInvolvedEquations(out_involvedEquations, false);
	}
}

void ECellEngine::Maths::Operation::LinkLocalOperands()
{
	ShrinkLocalOperands();
	PushOperands();
	for (std::vector<Operation>::iterator it = operations.begin(); it != operations.end(); it++)
	{
		it->LinkLocalOperands();
	}
}

void ECellEngine::Maths::Operation::InformStructureOfAddConstant() noexcept
{
	if (structure == OperationStructure_Empty)// 000 0 00 00
	{
		Data::Util::SetFlag(structure, OperationStructure_FirstOperandIsLocal);// 000 0 00 01
		if (operands.size() > 0)
		{
			Data::Util::SetFlag(structure, OperationStructure_FirstLocalOperandIsRHS); // 000 0 10 01
		}
	}
	else
	{
		Data::Util::SetFlag(structure, OperationStructure_SecondOperandIsLocal); // 000 0 XX 11
	}
}

void ECellEngine::Maths::Operation::InformStructureOfAddOperation() noexcept
{
	if (structure == OperationStructure_Empty)// 000 0 00 00
	{
		Data::Util::SetFlag(structure, OperationStructure_FirstOperandIsLocal);// 000 0 00 01 --> We add the first operand
		Data::Util::SetFlag(structure, OperationStructure_FirstLocalOperandIsOperation);// 000 0 01 01 --> The first operand is an Operation

		//if there is already one element in operands,
		//we register the operation as the second operand.
		// --> The bit 4 (so idx 3) is set to 1
		if (operands.size() > 0)
		{
			Data::Util::SetFlag(structure, OperationStructure_FirstLocalOperandIsRHS);// 000 0 11 01
		}
	}
	else
	{
		Data::Util::SetFlag(structure, OperationStructure_SecondOperandIsLocal);// 000 0 XX 11 --> We add the second operand
		Data::Util::SetFlag(structure, OperationStructure_SecondLocalOperandIsOperation);// 000 1 XX 11 --> The second operand is an Operation
	}
}

void ECellEngine::Maths::Operation::OverrideOperand(Operand* _operand, const unsigned char _idx) noexcept
{
	if (_idx < 2)
	{
		operands[_idx] = _operand;
	}
	else
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError(
			"ECellEngine::Maths::Operation::OverrideOperand(Operand*, const unsigned char): _idx must be 0 or 1. _idx = " + std::to_string(_idx) + ". Continuing without overriding.");
	}
}

void ECellEngine::Maths::Operation::PushOperands()
{
	// PushOperands is called for the 1st time here, and 
	// if there is at least 1 operand placement to decode
	if (!Data::Util::IsFlagSet(structure, OperationStructure_IsCompiled) && 
		  Data::Util::IsFlagSet(structure, OperationStructure_FirstOperandIsLocal))
	{
		// if the first operand is an Operation
		if (Data::Util::IsFlagSet(structure, OperationStructure_FirstLocalOperandIsOperation))
		{
			//((structure >> 3) & 1) indicates the operand (Operation or Cst) shall be
			//inserted at the index 0 or 1. We add operands.begin() to convert to an
			//iterator. 
			//&operations[0] is the pointer to the first Operation. It necessarily is
			//the first operation since we are decoding the first Operand in structure.
			operands.insert(operands.begin() + ((structure >> 3) & 1), &operations[0]);
		}
		else // then it is the code for a constant
		{
			operands.insert(operands.begin() + ((structure >> 3) & 1), &constants[0]);
		}

		if (Data::Util::IsFlagSet(structure, OperationStructure_SecondOperandIsLocal))//if the second operand must also be decoded
		{
			if (Data::Util::IsFlagSet(structure, OperationStructure_SecondLocalOperandIsOperation))// if this is the code for Operation
			{
				//This is the second operand so there must have been a first one.
				//Therefore, the operand to be decoded is in the second place (so
				// we can use operands.push_back()
				//Also, we do not care whether is is the first or second operation
				//to be included. On thing for sure is that it is at the back of the
				//operation vector. So we can use operations.back().
				operands.push_back(&operations.back());
			}
			else // then it is the code for a constant
			{
				operands.push_back(&constants.back());
			}
		}
	}
	Data::Util::SetFlag(structure, OperationStructure_IsCompiled);// 000 1 XX XX --> The Operation has been compiled: PushOperands has been called
}

void ECellEngine::Maths::Operation::UpdateFunction() noexcept
{
	switch (functionType)
	{
		case FunctionType_Identity:
			function = &functions.identity;
			break;
		case FunctionType_Plus:
			function = &functions.plus;
			break;
		case FunctionType_Minus:
			function = &functions.minus;
			break;
		case FunctionType_Times:
			function = &functions.times;
			break;
		case FunctionType_Divide:
			function = &functions.divide;
			break;
		case FunctionType_Power:
			function = &functions.power;
			break;
		case FunctionType_Root:
			function = &functions.root;
			break;
		default:
			//This should never happen
			//Are you sure you have the correct value for _functionType?
			//You didn't cast a random int to FunctionType, did you?
			assert(false);
			break;
	}
}

void ECellEngine::Maths::Operation::UpdateOperands()
{
	if (Data::Util::IsFlagSet(structure, OperationStructure_FirstOperandIsLocal)) //if there is at least 1 operand placement to decode
	{
		if (Data::Util::IsFlagSet(structure, OperationStructure_FirstLocalOperandIsOperation)) // if this is the code for Operation
		{
			//((structure >> 3) & 1) indicates the operand (Operation or Cst) shall be
			//inserted at the index 0 or 1.
			//&operations[0] is the pointer to the first Operation. It necessarily is
			//the first operation since we are decoding the first Operand in structure.
			operands[((structure >> 3) & 1)] = &operations[0];
		}
		else // then it is the code for a constant
		{
			operands[((structure >> 3) & 1)] = &constants[0];
		}

		if (Data::Util::IsFlagSet(structure, OperationStructure_SecondOperandIsLocal)) //if the second operand must also be decoded
		{
			if (Data::Util::IsFlagSet(structure, OperationStructure_SecondLocalOperandIsOperation)) // if this is the code for Operation
			{
				//This is the second operand so there must have been a first one.
				//Therefore, the operand to be decoded is in the second place (so
				// we can use operands.push_back()
				//Also, we do not care whether is is the first or second operation
				//to be included. On thing for sure is that it is at the back of the
				//operation vector. So we can use operations.back().
				operands[1] = &operations.back();
			}
			else // then it is the code for a constant
			{
				operands[1] = &constants.back();
			}
		}
	}
}

void ECellEngine::Maths::Operation::UpdateLHS(const float _previousValue, const float _newValue) noexcept
{
	operands[0]->Set(_newValue);
	float newRes = (*function)(operands);

	if (_previousValue != newRes)
	{
		onOperandChange(previousResult, newRes);
	}

	if (newRes != newResult)
	{
		onResultChange(newResult, newRes);
	}

	previousResult = newResult;
	newResult = newRes;
}

void ECellEngine::Maths::Operation::UpdateRHS(const float _previousValue, const float _newValue) noexcept
{
	operands[1]->Set(_newValue);
	float newRes = (*function)(operands);

	if (_previousValue != newRes)
	{
		onOperandChange(previousResult, newRes);
	}

	if (newRes != newResult)
	{
		onResultChange(newResult, newRes);
	}

	previousResult = newResult;
	newResult = newRes;
}