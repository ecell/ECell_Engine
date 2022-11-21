#pragma once

#include <string>

#include "DataState.hpp"
#include "Operand.hpp"

namespace ECellEngine::Data
{
    struct Species : public Operand
    {
    private:
        std::string id;
        std::string name;

    public:
        Species(std::string _id, std::string _name) :
            id{ _id }, name{ _name }
        {

        }

        inline virtual float Get(const DataState& _dataState) const noexcept override
        {
            _dataState.GetSpecies(name);
        }

        inline const std::string& GetId() const
        {
            return id;
        }

        inline const std::string& GetName() const
        {
            return name;
        }

        inline void Set(DataState& _dataState, const float& _val)
        {
            _dataState.SetSpecies(name, _val);
        }

        inline void Increment(DataState& _dataState, const float& _inc)
        {
            float val = _dataState.GetSpecies(name);
            _dataState.SetSpecies(name, val + _inc);
        }

        inline void Decrement(DataState& _dataState, const float& _dec)
        {
            float val = _dataState.GetSpecies(name);
            _dataState.SetSpecies(name, val - _dec);
        }
    };
}