#pragma once

#include <memory>
#include <vector>

#include "Function.hpp"

namespace ECellEngine::Maths
{
    struct Operation : public Operand
    {
    private:
        Function* function;
        std::vector<Operand*> operands;

    public:
        Operation() = default;

        Operation(const std::string _name) : 
            Operand (_name)
        {

        }

        inline void AddOperand(Operand& _operand)
        {
            operands.push_back(&_operand);
        }

        inline virtual float Get() const noexcept override
        {
            return (*function)(operands);
        }

        inline void Set(Function* _function) noexcept
        {
            function = _function;
        }
    };
}