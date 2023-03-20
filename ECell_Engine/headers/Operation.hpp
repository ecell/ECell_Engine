#pragma once

#include <memory>
#include <vector>

#include "Constant.hpp" //#include "Operand.hpp"
#include "Function.hpp"

namespace ECellEngine::Maths
{
    /*!
    @brief The data struct to describe a mathematical operation.
    @details It is designed for binary operations. A, operation may have other
             operations as its operands.
    */
    struct Operation : public Operand
    {
    private:
        /*!
        @brief The function to perform corresponding to the Operation.
        */
        const Function* function;

        /*!
        @brief A byte used to encode how the operation was built.
        @details It is typically usefull if the operation uses other operations
                 or numeric constants as its operands. Indeed, as constants and
                 operations are stored locally in ::constants and ::operations
                 (as opposed to global variables that are stored in the datastate),
                 we need to ascertain which of the two is the left and right operand.
                 In the parsing process, we assume that left operands are added first
                 and the right operands are added last. Therefore, we use 5 bits out
                 of the 8 to describe if one or two local operands are used and which
                 one was it in which order. The relevant bits are grouped as: 765 4 32 10.
                 We do not use the 5th, 6th and 7th bits.
                 
                 Bits 0 and 1 are used to indicate whether one (Bit 0 = 1) or
                 two (Bit 0 = 1 & Bit 1 = 1) local operands are used.
                 
                 Bit 2 indicates whether the 1st local operand is a Constant
                 (Bit 2 = 0) or another operation (Bit 2 = 0).

                 Bit 3 indicates whether the 1st local operand is the left operand
                 (Bit 3 = 0) or the right operand (Bit 3 = 1). This bit is important
                 to detect if a global variable from the datastate has already
                 been added as a left operand to the operation.
                 
                 Finally, Bit 4 is equivalent to Bit 2 (distinguish between local
                 constant or another operation) but specifically when a local
                 operand has already been added as the left operand.

        @see The source code of ::AddConstant and ::AddOperation for a demonstration
             of how this byte is written.
        @see The source code of ::PushOperands for a demonstration of how this byte
             is read.
        */
        unsigned char structure;

        /*!
        @brief The list of local constants used in the operation.
        */
        std::vector<Constant> constants;

        /*!
        @brief The list of local operations used in this operation.
        @details This is equivalent to children operations which builds a tree
                 when taken all together.
        */
        std::vector<Operation> operations;

        /*!
        @brief The list of right and left operands used in this operation.
        @details It may contain pointers to elements in ::constants, ::operations
                 or another variable in the datastate.
        */
        std::vector<Operand*> operands;

        /*!
        @brief Decodes ::structure to know in which order operands stored
               in ::constants or ::operations must be added in ::operands.
        */
        void PushOperands();

        /*!
        @brief Shrinks the capacity of the vectors ::constants and
                ::operations.
        @details This is relevant to save space once we know exactly
                 how many constants or other operations are used as
                 operands of this operation.
        */
        inline void ShrinkLocalOperands() noexcept
        {
            constants.shrink_to_fit();
            operations.shrink_to_fit();
        }

    public:
        Operation(const std::string _name) : 
            Operand (_name)
        {
            function = nullptr;
            structure = 0;
            constants.reserve(2);
            operations.reserve(2);
            operands.reserve(2);
        }

        /*!
        @brief Adds a constant in ::constants from its @p _value
        @details Also writes in ::structure to encode this new operand's
                 information.
        */
        inline Constant& AddConstant(const float _value)
        {
            constants.push_back(Constant(_value));

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

            return constants.back();
        }

        /*!
        @brief Adds an operand in ::operands.
        @details Mostly to use when adding a global variable stored in the 
                 datastate as an operand in this operation.
        */
        inline void AddOperand(Operand* _operand)
        {
            operands.push_back(_operand);
        }

        /*!
        @brief Adds an operation in ::operations from its @p _name
        @details Also writes in ::structure to encode this new operand's
                 information.
        */
        inline Operation& AddOperation(const std::string _name)
        {
            operations.push_back(Operation(_name));

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

            return operations.back();
        }

        /*!
        @brief Retrieves and fills the list @p _operandsNames with the names of
               all the operands' names of this operation (and the whole tree).
        @paramt OperandType A specific type of operand to chose from. It MUST
                derivates from ECellEngine::Maths::Operand.
        @param _operandsNames The reference to the vector where to store the
                names of the target operands.
        */
        template<typename OperandType>//, typename = std::enable_if_t<std::is_base_of_v<Operand, OperandType>>>
        void GetOperandsNames(std::vector<std::string>& _operandsNames) const noexcept
        {
            for (std::vector<Operand*>::const_iterator it = operands.begin(); it != operands.end(); it++)
            {
                if (dynamic_cast<OperandType*>(*it) != nullptr)
                {
                    _operandsNames.push_back((*it)->name);
                }
            }

            for (std::vector<Operation>::const_iterator it = operations.begin(); it != operations.end(); it++)
            {
                it->GetOperandsNames<OperandType>(_operandsNames);
            }
        }

        /*!
        @brief Gets the result of this operation.
        @details Uses ::function and ::operands to compute the operation.
        */
        inline virtual float Get() const noexcept override
        {
            return (*function)(operands);
        }

        /*!
        @brief Checks if there is at least two operands stored in this operation.
        @returns True if the sum of the size of ::operands, ::operations and
                 ::constants is greater or equal to 2.
        */
        inline bool IsFull()
        {
            return (operands.size() + operations.size() + constants.size() >= 2);
        }

        /*!
        @brief Recursively pushes operands in ::operands and trims unsused
                memory space for every child operation of this operation.
        @details Uses ::PushOperands and ::ShrinkLocalOperands.
        */
        void LinkLocalOperands();

        /*!
        @brief Sets the ::function that will be called when computing this
                operation.
        @param _function The pointer to the relevant mathematical function
                to link this operation.
        @see ECellEngine::Maths::Function.
        */
        inline void Set(const Function* _function) noexcept
        {
            function = _function;
        }

        
    };
}