#pragma once

#include <cmath>
#include <vector>

#include "Maths/Operand.hpp"

namespace ECellEngine::Maths
{
	struct Function
	{
        /*!
        @brief Computes the value of the function.
        @param _operands The operands of the function. At most constains 2
                elements for the left and right hand side of a binary function.
        */
		virtual float operator()(const std::vector<Operand*>& _operands) const noexcept = 0;

        /*!
        @brief Writes the string representation of the function.
        @param _operands The operands of the function. At most constains 2
                elements for the left and right hand side of a binary function.
        @returns The string representation of the function.
        */
        virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept = 0;
        
        /*!
        @brief Writes the string representation of the values of function's operands and result.
        @param _operands The operands of the function. At most constains 2
                elements for the left and right hand side of a binary function.
		@returns The string representation of the values of function's operands and result.
        */
        virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept = 0;
    };

    struct Add final : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() + _operands[1]->Get();
        }

        inline virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept override
        {
            return '(' + _operands[0]->ToString() + '+' + _operands[1]->ToString() + ')';
        }
        
        inline virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept override
        {
            return '(' + _operands[0]->ToStringValue() + '+' + _operands[1]->ToStringValue() + ')';
        }
    };

    struct Minus final : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() - _operands[1]->Get();
        }

        inline virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept override
        {
			return '(' + _operands[0]->ToString() + '-' + _operands[1]->ToString() + ')';
		}
        
        inline virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept override
        {
			return '(' + _operands[0]->ToStringValue() + '-' + _operands[1]->ToStringValue() + ')';
		}
    };

    struct Times final : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() * _operands[1]->Get();
        }

        inline virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->ToString() + '*' + _operands[1]->ToString();
        }
        
        inline virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->ToStringValue() + '*' + _operands[1]->ToStringValue();
        }
    };

    struct Divide final : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() / _operands[1]->Get();
        }

        inline virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept override
        {
			return _operands[0]->ToString() + '/' + _operands[1]->ToString();
		}
        
        inline virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept override
        {
			return _operands[0]->ToStringValue() + '/' + _operands[1]->ToStringValue();
		}
    };

    struct Power final : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return std::pow(_operands[0]->Get(), _operands[1]->Get());
        }

        inline virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->ToString() + '^' + _operands[1]->ToString();
        }
        
        inline virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->ToStringValue() + '^' + _operands[1]->ToStringValue();
        }
    };

    struct Identity final : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get();
        }

        inline virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept override
        {
			return _operands[0]->ToString();
		}
        
        inline virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept override
        {
			return _operands[0]->ToStringValue();
		}
    };

    struct Root final : public Function
    {
        /*!
        @brief Computes the root of a number.
        @details The degree of the root is the first operand; 
                 the number to compute the root of is the second operand.
        */
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return std::pow(_operands[1]->Get(), 1.0f / _operands[0]->Get());
        }

        inline virtual const std::string ToString(const std::vector<Operand*>& _operands) const noexcept override
        {
			return "rt(" + _operands[0]->ToString() + _operands[1]->ToString() + ")";
		}
        
        inline virtual const std::string ToStringValue(const std::vector<Operand*>& _operands) const noexcept override
        {
			return "rt(" + _operands[0]->ToStringValue() + _operands[1]->ToStringValue() + ")";
		}
    };

    struct Functions
    {
        static Add add;
        static Minus minus;
        static Times times;
        static Divide divide;
        static Power power;
        static Identity identity;
        static Root root;
    };

    static Functions functions;
}