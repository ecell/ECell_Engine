#pragma once

#include "Maths/Operand.hpp"

namespace ECellEngine::Data
{
	struct Parameter final : public ECellEngine::Maths::Operand
	{
	private:
		float initialValue;
		float previousValue;
		float value;

	public:
		std::shared_ptr<std::function<void(const float, const float)>> updateValueSubToken = nullptr;

		Parameter(const std::string& _name, const std::size_t _id, const float _value) :
			ECellEngine::Maths::Operand{ _name, _id}, value{ _value }, previousValue{ _value }, initialValue{ _value }
		{

		}

		inline float Get() const noexcept override
		{
			return value;
		}

		inline float GetInitialValue() const noexcept
		{
			return initialValue;
		}

		void GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector = true) const noexcept override;

		void Set(const float _value) noexcept override;

		inline void SetInitialValue(const float _value) noexcept
		{
			initialValue = _value;
		}

		float Reset() noexcept override;

		void UpdateValue(const float _previousValue, const float _newValue) noexcept;

		inline const std::string ToString() const noexcept override
		{
			return "P:"+name;
		}
		
		inline const std::string ToStringValue() const noexcept override
		{
			return "P:"+name+":"+std::to_string(value);
		}
	};
}