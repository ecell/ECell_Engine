#pragma once

#include <cmath>
#include <memory>

#include "DataState.hpp"
#include "Operand.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Maths
{
	struct Function
	{
		virtual float operator()(DataState const& _datastate, std::vector<std::shared_ptr<Operand>> const& _operands) const noexcept = 0;
	};

    struct Add : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<std::shared_ptr<Operand>> const& _operands) const noexcept override
        {
            return _operands[0].get()->Get(_datastate) + _operands[1].get()->Get(_datastate);
        }
    };

    struct Minus : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<std::shared_ptr<Operand>> const& _operands) const noexcept override
        {
            return _operands[0].get()->Get(_datastate) - _operands[1].get()->Get(_datastate);
        }
    };

    struct Times : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<std::shared_ptr<Operand>> const& _operands) const noexcept override
        {
            return _operands[0].get()->Get(_datastate) * _operands[1].get()->Get(_datastate);
        }
    };

    struct Divide : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<std::shared_ptr<Operand>> const& _operands) const noexcept override
        {
            return _operands[0].get()->Get(_datastate) / _operands[1].get()->Get(_datastate);
        }
    };

    struct Power : public Function
    {
        inline virtual float operator()(DataState const& _datastate, std::vector<std::shared_ptr<Operand>> const& _operands) const noexcept override
        {
            return std::pow(_operands[0].get()->Get(_datastate), _operands[1].get()->Get(_datastate));
        }
    };

    struct Functions
    {
        static Add add;
        static Minus minus;
        static Times times;
        static Divide divide;
        static Power power;
    };

    static Functions functions;
}