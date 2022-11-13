#pragma once

#include <string>

#include "DataState.hpp"
#include "Operand.hpp"

namespace ECellEngine::Data
{
    struct Parameter : public Operand
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
    
    struct SimpleParameter : public Parameter
    {
    public:
        SimpleParameter(std::string _name) :
            Parameter(_name)
        {

        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            _dataState.GetParameter(name);
        }

        inline virtual void Set(DataState& _dataState, const float& _value)
        {
            _dataState.SetParameter(name, _value);
        }
    };
    
    struct ComputedParameter : public Parameter
    {
    private:
        Operation operation;

    public:
        ComputedParameter(std::string _name) :
            Parameter(_name)
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