#pragma once

#include "Maths/Operand.hpp"

namespace ECellEngine::Data
{
	struct Parameter : public ECellEngine::Maths::Operand
	{
	private:
		float previousValue;
		float value;

	public:
		std::shared_ptr<std::function<void(const float, const float)>> updateValueSubToken = nullptr;

		Parameter(const std::string _name, const float _value) :
			ECellEngine::Maths::Operand{ _name }, value{ _value }, previousValue{ _value }
		{

		}

		inline virtual float Get() const noexcept override
		{
			return value;
		}

		virtual void Set(const float _value) noexcept override;

		void GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector = true) const noexcept override;

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