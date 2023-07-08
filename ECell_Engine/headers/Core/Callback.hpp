#pragma once

#include <functional>
#include <vector>

namespace ECellEngine::Core
{
	template<typename ... Args>
	class Callback
	{
		std::vector<std::function<void(Args ...)>> subsribers;

	public:
		Callback() = default;
		~Callback() = default;

		void operator+=(std::function<void(Args ...)> _subscriber)
		{
			subsribers.push_back(_subscriber);
		}

		void operator-=(std::function<void(Args ...)> _subscriber)
		{
			for (auto it = subsribers.begin(); it != subsribers.end(); ++it)
			{
				if (*it == _subscriber)
				{
					subsribers.erase(it);
					break;
				}
			}
		}

		void operator()(Args ... args)
		{
			for (auto& subscriber : subsribers)
			{
				subscriber(args ...);
			}
		}

		void Subscribe(std::function<void(Args ...)> _subscriber)
		{
			subsribers.push_back(_subscriber);
		}

		void Unsuscribe(std::function<void(Args ...)> _subscriber)
		{
			for (auto it = subsribers.begin(); it != subsribers.end(); ++it)
			{
				if (*it == _subscriber)
				{
					subsribers.erase(it);
					break;
				}
			}
		}
	};
}