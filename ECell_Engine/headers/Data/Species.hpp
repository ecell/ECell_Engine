#pragma once

#include <string>

#include "Maths/Operand.hpp"

namespace ECellEngine::Data
{
	struct Species final : public ECellEngine::Maths::Operand
	{
	private:
		float initialQuantity;
		float previousQuantity;
		float quantity;

	public:
		std::shared_ptr<std::function<void(const float, const float)>> updateQuantitySubToken = nullptr;

		Species(const std::string _name, const std::size_t _id, const float _quantity) :
			ECellEngine::Maths::Operand{ _name, _id }, quantity{_quantity}, previousQuantity{_quantity}, initialQuantity{_quantity}
		{

		}

		inline float Get() const noexcept override
		{
			return quantity;
		}

		inline float GetInitialQuantity() const noexcept
		{
			return initialQuantity;
		}

		void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept override;

		void Set(const float _val) noexcept override;

		void SetInitialQuantity(const float _initialQuantity) noexcept;

		void Increment(const float _inc);

		void Decrement(const float _dec);

		float Reset() noexcept override;

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