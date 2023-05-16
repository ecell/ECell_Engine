#pragma once

#include "Operand.hpp"

namespace ECellEngine::Data
{
    class Constant : public ECellEngine::Maths::Operand
    {
    private:
        float value;

    public:
        Constant(float _value) :
            ECellEngine::Maths::Operand(""), value{ _value }
        {
        }

        virtual float Get() const noexcept override
        {
            return value;
        }
    };
}