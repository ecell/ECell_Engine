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
	if (structure == 0)// 000 0 00 00
	{
		structure |= 1 << 0;// 000 0 00 01
		if (operands.size() > 0)
		{
			structure |= 1 << 3;// 000 0 10 01
		}
	}
	else
	{
		structure |= 1 << 1;// 000 0 XX 11
	}
}

void ECellEngine::Maths::Operation::InformStructureOfAddOperation() noexcept
{
	if (structure == 0)// 000 0 00 00
	{
		structure |= 1 << 0;// 000 0 00 01 --> We add the first operand
		structure |= 1 << 2;// 000 0 01 01 --> The first operand is an Operation

		//if there is already one element in operands,
		//we register the operation as the second operand.
		// --> The bit 4 (so idx 3) is set to 1
		if (operands.size() > 0)
		{
			structure |= 1 << 3;// 000 0 11 01
		}
	}
	else
	{
		structure |= 1 << 1;// 000 0 XX 11 --> We add the second operand
		structure |= 1 << 4;// 000 1 XX 11 --> The second operand is an Operation
	}
}

void ECellEngine::Maths::Operation::PushOperands()
{
	// PushOperands is called for the 1st time here, and 
	// if there is at least 1 operand placement to decode
	if (((structure >> 5) ^ 1) & ((structure >> 0) & 1)) 
	{
		if ((structure >> 2) & 1) // if this is the code for Operation
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

		if ((structure >> 1) & 1) //if the second operand must also be decoded
		{
			if ((structure >> 4) & 1) // if this is the code for Operation
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
	structure |= 1 << 5;// 001 X XX XX --> PushOperands has been called
}

void ECellEngine::Maths::Operation::UpdateOperands()
{
	if ((structure >> 0) & 1) //if there is at least 1 operand placement to decode
	{
		if ((structure >> 2) & 1) // if this is the code for Operation
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

		if ((structure >> 1) & 1) //if the second operand must also be decoded
		{
			if ((structure >> 4) & 1) // if this is the code for Operation
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