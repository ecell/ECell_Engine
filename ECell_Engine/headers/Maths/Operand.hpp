#pragma once

/*!
@file Operand.hpp
@brief Describes the base class for many data structures used in the simulation.
*/

#include <string>
#include <vector>
#include <string>

#include "Core/Callback.hpp"

namespace ECellEngine::Maths
{
	/*!
	@brief The base class for many data structures used in the simulation.
	@details Typically used to provide a common interface for the operands
			 used in the equations. Also used to harmonize the structure and
			 behaviour of elements in the data state.
	@see ECellEngine::Maths::Operation ECellEngine::Maths::Equation ECellEngine::Data::Parameter
		 ECellEngine::Data::Species ECellEngine::Data::Constant ECellEngine::Data::DataState
	*/
	struct Operand
	{
	private:
		/*!
		@brief The unique identifier of the operand.
		@remarks Compared to other usage of the IDProvider, the ID here is private
				to manage accessibility and constness rather than to declare it
				const directly. This is mandatory to avoid issues with deletion
				of assignement operators.
		*/
		std::size_t id;

	public:
		/*!
		@brief The name of the operand.
		*/
		std::string name;

		/*!
		@brief The callback called when the operand's value changes.
		*/
		Core::Callback<const float, const float> onValueChange;

		Operand(const std::size_t _id) :
			id{ _id }
		{

		}

		Operand(const std::string& _name, const std::size_t _id) :
			name{_name}, id{_id}
		{

		}

		virtual ~Operand() noexcept = default;

		inline bool operator==(const Operand& _other) { return Get() == _other.Get(); }
		inline bool operator!=(const Operand& _other) { return !(Get() == _other.Get()); }

		inline bool operator< (const Operand& _other) { return Get() < _other.Get(); }
		inline bool operator> (const Operand& _other) { return _other.Get() < Get(); }
		inline bool operator<=(const Operand& _other) { return !(Get() > _other.Get()); }
		inline bool operator>=(const Operand& _other) { return !(Get() < _other.Get()); }

		/*!
		@brief Accessors to the operand's value.
		@details The declaration of the value is left to the derived classes.
		*/
		virtual float Get() const noexcept = 0;

		/*!
		@brief Retrieve all the equations involved in this operand.

		@param out_involvedEquations The equations vector filled by the function call.
		@param clearOutVector Should the provided vector be cleared before filling.
		*/
		virtual void GetInvolvedEquations(std::vector<std::string>& out_involvedEquations, bool clearOutVector = true) const noexcept;
		
		/*!
		@brief Retrieve all the parameters involved in this operand.

		@param out_involvedParameters The parameters vector filled by the function call.
		@param clearOutVector Should the provided vector be cleared before filling.
		*/
		virtual void GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector = true) const noexcept;

		/*!
		@brief Retrieve all the species involved in this operand.

		@param out_involvedSpecies The species vector filled by the function call.
		@param clearOutVector Should the provided vector be cleared before filling.
		*/
		virtual void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept;

		/*!
		@brief Returns the unique identifier of the operand.
		@returns The unique identifier of the operand.
		*/
		std::size_t GetID() const noexcept
		{
			return id;
		}

		/*!
		@brief Sets the operand's value.
		@details The declaration of the value is left to the derived classes.
		*/
		virtual void Set(const float _value) noexcept = 0;

		/*!
		@brief Resets the operand's value.
		@details The declaration of the value is left to the derived classes.
		@return The new value of the operand.
		*/
		virtual float Reset() noexcept = 0;

		/*!
		@brief Generates the string representation of the operand.
		*/
		virtual const std::string ToString() const noexcept = 0;

		/*!
		@brief Generates the string representation of the operand's value.
		*/
		virtual const std::string ToStringValue() const noexcept = 0;
	};
}