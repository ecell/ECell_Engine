#pragma once

#include <string>
#include <vector>

#include "DataState.hpp"
#include "Function.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Maths
{
    struct Operand
    {
        virtual float Get(const DataState& _dataState) const noexcept = 0;
    };

    struct Operation : public Operand
    {
        Function* function;
        std::vector<Operand*> operands;

        Operation()
        {

        }

        inline void AddOperand(Operand* operand)
        {
            operands.emplace_back(operand);
        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            return (*function)(_dataState, operands);
        }

        inline void Set(Function* _function)
        {
            function = _function;
        }
    };
}