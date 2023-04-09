#pragma once

#include <string>

#include "Operand.hpp"

namespace ECellEngine::Data
{
    struct Parameter : public ECellEngine::Maths::Operand
    {
        Parameter(const std::string _name) :
            ECellEngine::Maths::Operand(_name)
        {

        }

        virtual float Get() const noexcept override = 0;
    };
}