#pragma once

#include "Parameter.hpp"
#include "Operation.hpp"

namespace ECellEngine::Data
{
    struct ComputedParameter : public Parameter
    {
    private:
        float valueCache;
        ECellEngine::Maths::Operation operation;

    public:
        ComputedParameter(const std::string _name, const ECellEngine::Maths::Operation _operation) :
            Parameter(_name), operation{ _operation }
        {

        }

        inline float ComputeOperation()
        {
            valueCache = operation.Get();
            return valueCache;
        }

        inline virtual float Get() const noexcept override
        {
            return valueCache;
        }

        inline const ECellEngine::Maths::Operation& GetOperation() noexcept
        {
            return operation;
        }

        void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept override;
    };
}