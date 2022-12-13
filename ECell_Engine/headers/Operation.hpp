#pragma once

#include <memory>
#include <vector>

#include "Operand.hpp"
#include "Function.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Maths
{
    struct Operation : public Operand
    {
    private:
        Function* function;
        std::vector <std::shared_ptr<Operand>> operands;

    public:
        Operation() = default;

        inline void AddOperand(const std::shared_ptr<Operand> operand)
        {
            operands.emplace_back(operand);
        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            return (*function)(_dataState, operands);
        }

        inline void Set(Function* _function) noexcept
        {
            function = _function;
        }
    };
}