#pragma once

#include "DataState.hpp"

namespace ECellEngine::Maths
{
    struct Operand
    {
        const std::string name;

    public:
        Operand(const std::string _name) :
            name{_name}
        {

        }

        virtual float Get(const ECellEngine::Data::DataState& _dataState) const noexcept = 0;
    };
}