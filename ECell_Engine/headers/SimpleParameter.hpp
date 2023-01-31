#pragma once

#include "Parameter.hpp"

namespace ECellEngine::Data
{
    struct SimpleParameter : public Parameter
    {
    private:
        float value;

    public:
        SimpleParameter(const std::string _name, const float _value) :
            Parameter(_name), value{_value}
        {

        }

        inline virtual float Get() const noexcept override
        {
            return value;
        }

        inline void Set(const float& _value) noexcept
        {
            value = _value;
        }
    };
}