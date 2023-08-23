#pragma once

#include "Logging/Logger.hpp"
#include "Maths/Operand.hpp"

namespace ECellEngine::Data
{
    /*!
    @brief A struct to encapsulate a constant value.
    @details This is useful to create the tree of operations.
    @see ECellEngine::Maths::Operation
    */
    class Constant final : public ECellEngine::Maths::Operand
    {
    private:
        /*!
        @brief The value of the constant.
        */
        float value;

    public:
        /*!
        @brief Creates a new constant with value @p _value.
        @param _value The value of the constant.
        @details The name of the constant is set to the empty string.
        The id of the constant is set to SIZE_MAX because we will never
        need to identify a constant so the id is not important.
        */
        Constant(float _value) :
            ECellEngine::Maths::Operand(""), value{ _value }
        {
        }

        float Get() const noexcept override
        {
            return value;
        }

        /*
        @brief DO NOT USE for this class.
        @details Does nothing; simply raises an error in the logger.
		*/
        inline void Set(const float _val) noexcept override
        {
            ECellEngine::Logging::Logger::LogError("Operation::Set(const float _value) should not be used.");
		}

        inline const std::string ToString() const noexcept override
        {
			return std::to_string(value);
		}
        
        inline const std::string ToStringValue() const noexcept override
        {
			return std::to_string(value);
		}
    };
}