#pragma once

#include "Parameter.hpp"

namespace ECellEngine::Data
{
    struct SimpleParameter : public Parameter
    {
    public:
        SimpleParameter(std::string _name) :
            Parameter(_name)
        {

        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            _dataState.GetParameter(name);
        }

        inline virtual void Set(DataState& _dataState, const float& _value)
        {
            _dataState.SetParameter(name, _value);
        }
    };
}