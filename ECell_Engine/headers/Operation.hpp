#pragma once

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

        template<typename OperandType>//, typename = std::enable_if_t<std::is_base_of_v<Operand, OperandType>>>
        void GetOperandsNames(std::vector<std::string>& _operandsNames) const noexcept
        {
            for (std::vector<Operand*>::const_iterator it = operands.begin(); it != operands.end(); it++)
            {
                if (dynamic_cast<OperandType*>(*it) != nullptr)
                {
                    _operandsNames.push_back((*it)->name);
                }

                Operation* op = dynamic_cast<Operation*>(*it);
                if (op != nullptr)
                {
                    op->GetOperandsNames<OperandType>(_operandsNames);
                }
            }
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