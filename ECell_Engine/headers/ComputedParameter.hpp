#pragma once

#include "Parameter.hpp"
#include "Operation.hpp"

namespace ECellEngine::Data
{
    struct ComputedParameter : public Parameter
    {
    private:
        Operation operation;

    public:
        ComputedParameter(std::string _name, Operation _operation) :
            Parameter(_name), operation{ _operation }
        {
        }

        inline void ComputeOperation(DataState& _dataState)
        {
            _dataState.SetParameter(name, operation.Get(_dataState));
        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            _dataState.GetParameter(name);
        }

    };
}