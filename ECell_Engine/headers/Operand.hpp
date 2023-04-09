#pragma once

#include <string>

namespace ECellEngine::Maths
{
    struct Operand
    {
        const std::string name;

        Operand(std::string _name) :
            name{_name}
        {

        }

        virtual float Get() const noexcept = 0;
    };
}