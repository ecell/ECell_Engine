#pragma once

#include <string>
#include <vector>
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

        virtual ~Operand() noexcept = default;

        virtual float Get() const noexcept = 0;

        /*!
        @brief Retrieve all the species involved in this operand.

        @param out_involvedSpecies The species vector filled by the function call.
        @param clearOutVector Should the provided vector be cleared before filling.
        */
        virtual void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept;

        /*!
        @brief Retrieve all the simple parameters involved in this operand.

        @param out_involvedSimpleParameters The simple parameters vector filled by the function call.
        @param clearOutVector Should the provided vector be cleared before filling.
        */
        virtual void GetInvolvedSimpleParameters(std::vector<std::string>& out_involvedSimpleParameters, bool clearOutVector = true) const noexcept;

        /*!
        @brief Retrieve all the computed parameters involved in this operand.

        @param out_involvedComputedParameters The simple parameters vector filled by the function call.
        @param clearOutVector Should the provided vector be cleared before filling.
        */
        virtual void GetInvolvedComputedParameters(std::vector<std::string>& out_involvedComputedParameters, bool clearOutVector = true) const noexcept;
    };
}