#pragma once

#include <string>

#include "Maths/Operand.hpp"

namespace ECellEngine::Data
{
	struct Species : public ECellEngine::Maths::Operand
	{
	private:
		float previousQuantity;
		float quantity;

	public:
		std::shared_ptr<std::function<void(const float, const float)>> updateQuantitySubToken = nullptr;

		Species(const std::string _name, const float _quantity) :
			ECellEngine::Maths::Operand{ _name }, quantity{_quantity}, previousQuantity{_quantity}
		{

		}

		inline virtual float Get() const noexcept override
		{
			return quantity;
		}

		void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept override;
		
		inline virtual void Set(const float _val) noexcept override
		{
			quantity = _val;
		}

		inline void Increment(const float _inc)
		{
			quantity += _inc;
		}

		inline void Decrement(const float _dec)
		{
			quantity -= _dec;
		}

		void UpdateQuantity(const float _previousValue, const float _newValue) noexcept;

		inline const std::string ToString() const noexcept override
		{
			return "Sp:" + name;
		}
		
		inline const std::string ToStringValue() const noexcept override
		{
			return "Sp:" + name + ":" + std::to_string(quantity);
		}
	};
}