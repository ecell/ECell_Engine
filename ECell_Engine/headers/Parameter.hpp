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

        inline std::string GetName() const
        {
            return name;
        }

    public:
        Parameter(std::string _name) :
            name{ _name }
        {

        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            _dataState.GetParameter(name);
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
    };
    
    struct ComputedParameter : public Parameter
    {
    public:
        ComputedParameter(std::string _name) :
            Parameter(_name)
        {
        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            _dataState.GetParameter(name);
        }
    };
}