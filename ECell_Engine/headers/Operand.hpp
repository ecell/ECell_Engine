#pragma once

#include "DataState.hpp"

namespace ECellEngine::Maths
{
    struct Operand
    {
        virtual float Get(const ECellEngine::Data::DataState& _dataState) const noexcept = 0;
    };

    struct Constant : public Operand
    {
    private:
        float value;

    public:
        Constant(float _value) :
            value{ _value }
        {
        }

        virtual float Get(const ECellEngine::Data::DataState& _dataState) const noexcept override
        {
            return value;
        }
    };
}