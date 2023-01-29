#pragma once

#include <string>

#include "DataState.hpp"
#include "Operand.hpp"

namespace ECellEngine::Data
{
    struct Parameter : public ECellEngine::Maths::Operand
    {
    public:
        Parameter(const std::string _name) :
            Operand(_name)
        {

        }

        inline virtual float Get(const DataState& _dataState) const noexcept = 0;
    };
}