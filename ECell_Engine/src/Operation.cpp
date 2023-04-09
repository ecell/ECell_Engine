#include "Operation.hpp"

void ECellEngine::Maths::Operation::LinkLocalOperands()
{
    ShrinkLocalOperands();
    PushOperands();
    for (std::vector<Operation>::iterator it = operations.begin(); it != operations.end(); it++)
    {
        it->LinkLocalOperands();
    }
}

void ECellEngine::Maths::Operation::PushOperands()
{
    if ((structure >> 0) & 1) //if there is at least 1 operand placement to decode
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
}

void ECellEngine::Maths::Operation::UpdateOperands()
{
    if ((structure >> 0) & 1) //if there is at least 1 operand placement to decode
    {
        if ((structure >> 2) & 1) // if this is the code for Operation
        {
            //((structure >> 3) & 1) indicates the operand (Operation or Cst) shall be
            //inserted at the index 0 or 1. We add operands.begin() to convert to an
            //iterator. 
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