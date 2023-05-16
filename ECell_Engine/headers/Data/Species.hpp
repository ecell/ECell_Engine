#pragma once

#include <string>

#include "Operand.hpp"

namespace ECellEngine::Data
{
	struct Species : public ECellEngine::Maths::Operand
	{
	private:
		float quantity;

	public:
		Species(const std::string _name, const float _quantity) :
			ECellEngine::Maths::Operand{ _name }, quantity{_quantity}
		{

		}

		inline virtual float Get() const noexcept override
		{
			return quantity;
		}

		inline void Set(const float& _val)
		{
			quantity = _val;
		}

		inline void Increment(const float& _inc)
		{
			quantity += _inc;
		}

		inline void Decrement(const float& _dec)
		{
			quantity -= _dec;
		}

		void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept override;
	};
}