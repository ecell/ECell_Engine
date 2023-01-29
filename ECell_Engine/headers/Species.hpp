#pragma once

#include <string>

#include "DataState.hpp"
#include "Operand.hpp"

namespace ECellEngine::Data
{
    struct Species : public ECellEngine::Maths::Operand
    {
    public:
        Species(const std::string _name) :
            ECellEngine::Maths::Operand{ _name }
        {

        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            return _dataState.GetSpecies(name);
        }

        inline void Set(DataState& _dataState, const float& _val)
        {
            _dataState.SetSpecies(name, _val);
        }

        inline void Increment(DataState& _dataState, const float& _inc)
        {
            float val = _dataState.GetSpecies(name);
            _dataState.SetSpecies(name, val + _inc);
        }

        inline void Decrement(DataState& _dataState, const float& _dec)
        {
            float val = _dataState.GetSpecies(name);
            _dataState.SetSpecies(name, val - _dec);
        }
    };
}