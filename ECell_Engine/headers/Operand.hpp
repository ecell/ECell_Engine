#pragma once

#include <memory>
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

    struct Constant : public Operand
    {
    private:
        float value;

    public:
        Constant(float _value) :
            value{ _value }
        {
        }

        virtual float Get(const DataState& _dataState) const noexcept override
        {
            return value;
        }
    };

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