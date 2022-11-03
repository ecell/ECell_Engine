#pragma once

#include <cmath>
#include "DataState.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Maths
{
	struct Function
	{
		virtual float operator()(DataState const& _datastate, std::vector<Operand*> const& _operands) const noexcept = 0;
	};

    struct Add : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<Operand*> const& _operands) const noexcept override
        {
            return _operands[0]->Get(_datastate) + _operands[1]->Get(_datastate);
        }
    };

    struct Sub : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<Operand*> const& _operands) const noexcept override
        {
            return _operands[0]->Get(_datastate) - _operands[1]->Get(_datastate);
        }
    };

    struct Mul : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<Operand*> const& _operands) const noexcept override
        {
            return _operands[0]->Get(_datastate) * _operands[1]->Get(_datastate);
        }
    };

    struct Div : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<Operand*> const& _operands) const noexcept override
        {
            return _operands[0]->Get(_datastate) / _operands[1]->Get(_datastate);
        }
    };

    struct Pow : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<Operand*> const& _operands) const noexcept override
        {
            return std::pow(_operands[0]->Get(_datastate), _operands[1]->Get(_datastate));
        }
    };
}