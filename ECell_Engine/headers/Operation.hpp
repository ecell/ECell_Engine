#pragma once

#include <memory>
#include <vector>

#include "Constant.hpp"
#include "Function.hpp"

namespace ECellEngine::Maths
{
    struct Operation : public Operand
    {
    private:
        const Function* function;
        std::vector<std::shared_ptr<Constant>> constants;
        std::vector<Operand*> operands;

    public:
        Operation() = default;

        Operation(const std::string _name) : 
            Operand (_name)
        {

        }

        inline void AddConstant(float _value)
        {
            constants.push_back(std::make_shared<Constant>(_value));
            operands.push_back(constants.back().get());
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

        inline void Set(const Function* _function) noexcept
        {
            function = _function;
        }
    };
}