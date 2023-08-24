#pragma once 

#include <iterator>

namespace ECellEngine::Util
{
	struct BinarySearch
	{
		/*!
		@brief Performs a binary search in [@p _first, @p _last] looking for
				@p _value.
		@details Uses comparison *it < @p _value between the elements.
		@param _first The first element of the range.
		@param _last The last element of the range.
		@param _value The value to look for.
		@return The iterator pointing to the first element in the range which
				does not satisfy *it < @p _value.
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
		@details Uses comparison comp(*it, @p _value) between the elements.
		@param _first The first element of the range.
		@param _last The last element of the range.
		@param _value The value to look for.
		@return The iterator pointing to the first element in the range which
				does not satisfy the comparison function.
		@remarks STD implementation of binary search. https://en.cppreference.com/w/cpp/algorithm/lower_bound
				 This is to avoid to include the whole <algorithm> header.
		*/
		template<class ForwardIt, class T, class Compare>
		static ForwardIt LowerBound(ForwardIt _first, ForwardIt _last, const T& _value, Compare _comp)
		{
			ForwardIt it;
			typename std::iterator_traits<ForwardIt>::difference_type count, step;
			count = std::distance(_first, _last);

			while (count > 0)
			{
				it = _first;
				step = count / 2;
				std::advance(it, step);

				if (_comp(*it, _value))
				{
					_first = ++it;
					count -= step + 1;
				}
				else
					count = step;
			}

			return _first;
		}

		/*!
		@brief Performs a binary search in [@p _first, @p _last] looking for
				@p _value.
		@details Uses comparison !(*it < @p _value) between the elements.
		@param _first The first element of the range.
		@param _last The last element of the range.
		@param _value The value to look for.
		@return The iterator pointing to the first element in the range which
				does not satisfy *it < @p _value.
		@remarks STD implementation of binary search. https://en.cppreference.com/w/cpp/algorithm/lower_bound
				 This is to avoid to include the whole <algorithm> header.
		*/
		template<class ForwardIt, class T>
		static ForwardIt UpperBound(ForwardIt _first, ForwardIt _last, const T& _value)
		{
			ForwardIt it;
			typename std::iterator_traits<ForwardIt>::difference_type count, step;
			count = std::distance(_first, _last);

			while (count > 0)
			{
				it = _first;
				step = count / 2;
				std::advance(it, step);

				if (!(_value < *it))
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
		@details Uses comparison !comp(*it, @p _value) between the elements.
		@param _first The first element of the range.
		@param _last The last element of the range.
		@param _value The value to look for.
		@return The iterator pointing to the first element in the range which
				does not satisfy the opposite of the comparison function.
		@remarks STD implementation of binary search. https://en.cppreference.com/w/cpp/algorithm/lower_bound
				 This is to avoid to include the whole <algorithm> header.
		*/
		template<class ForwardIt, class T, class Compare>
		static ForwardIt UpperBound(ForwardIt _first, ForwardIt _last, const T& _value, Compare _comp)
		{
			ForwardIt it;
			typename std::iterator_traits<ForwardIt>::difference_type count, step;
			count = std::distance(_first, _last);

			while (count > 0)
			{
				it = _first;
				step = count / 2;
				std::advance(it, step);

				if (!(_comp(_value, *it)))
				{
					_first = ++it;
					count -= step + 1;
				}
				else
					count = step;
			}

			return _first;
		}
	};


}