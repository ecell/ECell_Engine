#pragma once

#include "Logging/Logger.hpp"
#include "Maths/Operand.hpp"

namespace ECellEngine::Data
{
    class Constant : public ECellEngine::Maths::Operand
    {
    private:
        float value;

    public:
        Constant(float _value) :
            ECellEngine::Maths::Operand(""), value{ _value }
        {
        }

        virtual float Get() const noexcept override
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