#pragma once

#include <string>
#include <vector>
#include <string>

#include "Core/Callback.hpp"

namespace ECellEngine::Maths
{
    struct Operand
    {
        std::string name;

        Core::Callback<const float, const float> onValueChange;

        Operand() = default;

        Operand(std::string _name) :
            name{_name}
        {

        }

        virtual ~Operand() noexcept = default;

        inline bool operator==(const Operand& _other) { return Get() == _other.Get(); }
        inline bool operator!=(const Operand& _other) { return !(Get() == _other.Get()); }

        inline bool operator< (const Operand& _other) { return Get() < _other.Get(); }
        inline bool operator> (const Operand& _other) { return _other.Get() < Get(); }
        inline bool operator<=(const Operand& _other) { return !(Get() > _other.Get()); }
        inline bool operator>=(const Operand& _other) { return !(Get() < _other.Get()); }

        virtual float Get() const noexcept = 0;

        /*!
        @brief Retrieve all the equations involved in this operand.

        @param out_involvedEquations The equations vector filled by the function call.
        @param clearOutVector Should the provided vector be cleared before filling.
        */
        virtual void GetInvolvedEquations(std::vector<std::string>& out_involvedEquations, bool clearOutVector = true) const noexcept;
        
        /*!
        @brief Retrieve all the parameters involved in this operand.

        @param out_involvedParameters The parameters vector filled by the function call.
        @param clearOutVector Should the provided vector be cleared before filling.
        */
        virtual void GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector = true) const noexcept;

        /*!
        @brief Retrieve all the species involved in this operand.

        @param out_involvedSpecies The species vector filled by the function call.
        @param clearOutVector Should the provided vector be cleared before filling.
        */
        virtual void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept;

    
        virtual void Set(const float _value) noexcept = 0;

        /*!
        @brief Generates the string representation of the operand.
        */
        virtual const std::string ToString() const noexcept = 0;

        /*!
        @brief Generates the string representation of the operand's value.
        */
        virtual const std::string ToStringValue() const noexcept = 0;
    };
}