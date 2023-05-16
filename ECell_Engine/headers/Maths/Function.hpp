#pragma once

#include <cmath>
#include <vector>

#include "Maths/Operand.hpp"

namespace ECellEngine::Maths
{
	struct Function
	{
		virtual float operator()(const std::vector<Operand*>& _operands) const noexcept = 0;
    };

    struct Add : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() + _operands[1]->Get();
        }
    };

    struct Minus : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() - _operands[1]->Get();
        }
    };

    struct Times : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() * _operands[1]->Get();
        }
    };

    struct Divide : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get() / _operands[1]->Get();
        }
    };

    struct Power : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return std::pow(_operands[0]->Get(), _operands[1]->Get());
        }
    };

    struct Identity : public Function
    {
        inline virtual float operator()(const std::vector<Operand*>& _operands) const noexcept override
        {
            return _operands[0]->Get();
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
    };

    static Functions functions;
}