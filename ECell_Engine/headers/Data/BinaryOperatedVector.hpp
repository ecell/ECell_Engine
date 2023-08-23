#pragma once 

#include <vector>
#include <string>

namespace ECellEngine::Data
{
	struct BinaryOperation
	{
		/*!
		@brief Performs a binary search in [@p _first, @p _last] looking for
				@p _value.
		@remarks STD implementation of binary search. https://en.cppreference.com/w/cpp/algorithm/lower_bound
				 This is to avoid to include the whole <algorithm> header.
		*/
		template<class ForwardIt, class T>
		static ForwardIt LowerBound(ForwardIt _first, ForwardIt _last, const T& _value)
		{
			ForwardIt it;
			typename std::iterator_traits<ForwardIt>::difference_type count, step;
			count = std::distance(_first, _last);

			while (count > 0)
			{
				it = _first;
				step = count / 2;
				std::advance(it, step);

				if (*it < _value)
				{
					_first = ++it;
					count -= step + 1;
				}
				else
				{
					count = step;
				}
			}

			return _first;
		}

		/*!
		@brief Performs a binary search in [@p _first, @p _last] looking for
				@p _value under the binary comparison function @comp.
		@remarks STD implementation of binary search. https://en.cppreference.com/w/cpp/algorithm/lower_bound
				 This is to avoid to include the whole <algorithm> header.
		*/
		template<class ForwardIt, class T, class Compare>
		static ForwardIt LowerBound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
		{
			ForwardIt it;
			typename std::iterator_traits<ForwardIt>::difference_type count, step;
			count = std::distance(first, last);

			while (count > 0)
			{
				it = first;
				step = count / 2;
				std::advance(it, step);

				if (comp(*it, value))
				{
					first = ++it;
					count -= step + 1;
				}
				else
					count = step;
			}

			return first;
		}
	};


}