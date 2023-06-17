#pragma once

#include "Maths/Operand.hpp"

namespace ECellEngine::Data
{
	struct Parameter : public ECellEngine::Maths::Operand
	{
	private:
		float value;

	public:
		Parameter(const std::string _name, const float _value) :
			ECellEngine::Maths::Operand{ _name }, value{ _value }
		{

		}

		inline virtual float Get() const noexcept override
		{
			return value;
		}

		inline void Set(const float _value) noexcept override
		{
			value = _value;
		}

		void GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector = true) const noexcept override;

		inline const std::string ToString() const noexcept override
		{
			return "P:"+name;
		}
	};
}