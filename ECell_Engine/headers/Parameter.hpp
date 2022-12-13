#pragma once

#include <string>

#include "DataState.hpp"
#include "Operand.hpp"

namespace ECellEngine::Data
{
    struct Parameter : public ECellEngine::Maths::Operand
    {
    protected:
        std::string name;

    public:
        Parameter(std::string _name) :
            name{ _name }
        {

        }

        inline virtual float Get(const DataState& _dataState) const noexcept = 0;

        inline const std::string GetName() const
        {
            return name;
        }
    };
}