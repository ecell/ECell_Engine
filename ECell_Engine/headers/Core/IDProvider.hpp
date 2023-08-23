#pragma once

/*!
@file IDProvider.h
@brief Lightweight code to manage an 64-bits unsigned integer ID.
*/

#include <cstddef> // std::size_t

namespace ECellEngine::Core
{
	/*!
	@brief Encapsulates an 64-bits unsigned integer that can be used to provide
			unique IDs.
	@details Use the overriden operators to assign, get and pre-increment the ID.
	*/
	struct IDProvider
	{
		/*!
		@brief The ID.
		@details It is initialized to the maximum value of an unsigned long long
				so that the first increment using the ++ operator will return 0.
		*/
		unsigned long long id = 0xffffffffffffffff;

		/*!
		@brief Assigns the ID.
		@details You probably don't want to use this unless you are resetting in
				 an initialization function (e.g. during deserialization).
		*/
		void operator=(unsigned long long _id)
		{
			id = _id;
		}

		/*!
		@brief Gets the current value of the ID. DOES NOT increment the value
				internally.
		*/
		unsigned long long operator()() const
		{
			return id;
		}

		/*!
		@brief Increments the ID and returns the new value.
		*/
		unsigned long long& operator++()
		{
			return ++id;
		}

		operator std::size_t() const
		{
			return id;
		}
	};
}