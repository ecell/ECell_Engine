#pragma once
#include <string>

namespace ECellEngine::Maths
{
	/*!
	@brief Class used to compare two booleans.
	*/
	struct Logic
	{
		/*!
		@brief Computes the result of the logic operator.
		@param _lhs The left hand side of the logic operator.
		@param _rhs The right hand side of the logic operator.
		*/
		virtual bool operator()(const bool _lhs, const bool _rhs) const noexcept = 0;

		/*!
		@brief Writes the string representation of the logic comparison.
		@param _lhs The left hand side of the logic operator.
		@param _rhs The right hand side of the logic operator.
		@returns The string representation of the comparison.
		*/
		virtual const std::string ToString(const bool _lhs, const bool _rhs) const noexcept = 0;

		/*!
		@brief Writes the string representation of the result of the comparison.
		@param _lhs The left hand side of the logic operator.
		@param _rhs The right hand side of the logic operator.
		@returns The string representation of the result of comparison..
		*/
		virtual const std::string ToStringValue(const bool _lhs, const bool _rhs) const noexcept = 0;
	};

	/*!
	@brief The AND logic operator.
	*/
	struct And final : public Logic
	{
		inline virtual bool operator()(const bool _lhs, const bool _rhs) const noexcept override
		{
			return _lhs && _rhs;
		}
		inline virtual const std::string ToString(const bool _lhs, const bool _rhs) const noexcept override
		{
			std::string result = "(";
			result += _lhs ? "True AND " : "False AND";
			result += _rhs ? "True" : "False";
			return result + ")";
		}
		inline virtual const std::string ToStringValue(const bool _lhs, const bool _rhs) const noexcept override
		{
			std::string result = "(";
			result += (_lhs && _rhs) ? "True" : "False";
			return result + ")";
		}
	};

	/*!
	@brief The OR logic operator.
	*/
	struct Or final : public Logic
	{
		inline virtual bool operator()(const bool _lhs, const bool _rhs) const noexcept override
		{
			return _lhs || _rhs;
		}
		inline virtual const std::string ToString(const bool _lhs, const bool _rhs) const noexcept override
		{
			std::string result = "(";
			result += _lhs ? "True OR " : "False OR";
			result += _rhs ? "True" : "False";
			return result + ")";
		}
		inline virtual const std::string ToStringValue(const bool _lhs, const bool _rhs) const noexcept override
		{
			std::string result = "(";
			result += (_lhs || _rhs) ? "True" : "False";
			return result + ")";
		}
	};

	/*
	@brief The NOT logic operator. Uses only LHS in all member functions.
	*/
	struct Not final : public Logic
	{
		inline virtual bool operator()(const bool _lhs, const bool _rhs) const noexcept override
		{
			return !_lhs;
		}
		inline virtual const std::string ToString(const bool _lhs, const bool _rhs) const noexcept override
		{
			std::string result = "(NOT ";
			result += !_lhs ? "True" : "False";
			return result + ")";
		}
		inline virtual const std::string ToStringValue(const bool _lhs, const bool _rhs) const noexcept override
		{
			std::string result = "(NOT ";
			result += !_lhs ? "True" : "False";
			return result + ")";
		}
	};

	struct Logics
	{
		static And and;
		static Or or;
		static Not not;
	};

	static Logics logics;
}