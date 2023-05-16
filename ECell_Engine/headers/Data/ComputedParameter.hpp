#pragma once

#include "Data/Parameter.hpp"
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

        void GetInvolvedSimpleParameters(std::vector<std::string>& out_involvedSimpleParameters, bool clearOutVector = true) const noexcept override;

        void GetInvolvedComputedParameters(std::vector<std::string>& out_involvedComputedParameters, bool clearOutVector = true) const noexcept override;
    };
}