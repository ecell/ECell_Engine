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
	};
	/*
	struct BinaryOperatedSize_tVector
	{
		std::vector<std::size_t> v;

		BinaryOperatedSize_tVector() = default;

		BinaryOperatedSize_tVector(std::vector<std::size_t> _v) : v{ _v }
		{

		}

		inline auto begin()
		{
			return v.begin();
		}

		inline auto end()
		{
			return v.end();
		}

		inline void sortedInsert(const std::size_t _item)
		{
			std::size_t l = 0;
			std::size_t r = 0;
			if (v.size() > 0)
			{
				if (_item <= v[0])
				{
					v.insert(v.begin(), _item);
				}

				else if (v.back() <= _item)
				{
					v.insert(v.end(), _item);
				}

				else
				{
					r = v.size() - 1;

					while (l < r - 1)
					{
						std::size_t mid = (std::size_t)((l + r) * 0.5f);
						if (v[mid] < _item)
						{
							l = mid;
						}
						else
						{
							r = mid;
						}
					}
					v.insert(v.begin() + r, _item);
				}
			}
			else
			{
				v.insert(v.begin(), _item);
			}

		}
	};
	
	struct BinaryOperatedStrVector
	{
		std::vector<std::string> v;

		BinaryOperatedStrVector() = default;

		BinaryOperatedStrVector(std::vector<std::string> _v) : v{ _v }
		{

		}
		
		inline auto begin()
		{
			return v.begin();
		}

		inline auto end()
		{
			return v.end();
		}

		inline void sortedInsert(const std::string _item)
		{
			std::size_t l = 0;
			std::size_t r = 0;
			if (v.size() > 0)
			{
				std::string lowItem = str_tolower(_item);
				if (lowItem <= str_tolower(v[0]))
				{
					v.insert(v.begin(), _item);
				}

				else if (str_tolower(v.back()) <= lowItem)
				{
					v.insert(v.end(), _item);
				}

				else
				{
					r = v.size() - 1;

					while (l < r - 1)
					{
						std::size_t mid = (std::size_t)((l + r) * 0.5f);
						if (str_tolower(v[mid]) < lowItem)
						{
							l = mid;
						}
						else
						{
							r = mid;
						}
					}
					v.insert(v.begin() + r, _item);
				}
			}
			else
			{
				v.insert(v.begin(), _item);
			}

		}

		inline std::string str_tolower(std::string s)
		{
			for (int i = 0; i < s.size(); i++)
			{
				s[i] = std::tolower(s[i]);
			}
			return s;
		}
	};*/
}