#pragma once

#include "Operand.hpp"

namespace ECellEngine::Maths
{
    class Constant : public Operand
    {
    private:
        float value;

    public:
        Constant(float _value) :
            Operand(""), value{ _value }
        {
        }

        virtual float Get() const noexcept override
        {
            return value;
        }
    };
}