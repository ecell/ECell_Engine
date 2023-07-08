#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace ECellEngine::Core
{
	/*!
	@brief A callback system inpired by the C# one.
	@details The object making the subscription is responsible for keeping the returned
			 shared_ptr alive. It is used as a token to identify the subscription when we
			 want to unsubscribe. Also, if the object is destroyed, the callback will be
			 eventually removed from the list of subscribers.
	@tparam Args Types of the arguments of the callback.
	@remarks Heavily relying on https://stackoverflow.com/a/42938693 and
			 https://stackoverflow.com/a/34400239.
	*/
	template<typename ... Args>
	class Callback
	{
		std::vector<std::weak_ptr<std::function<void(Args ...)>>> subscribers;

	public:
		Callback() = default;
		~Callback() = default;

		std::shared_ptr<std::function<void(Args ...)>> operator+=(std::function<void(Args ...)> _subscriber)
		{
			//The deleter in the shared_ptr will be called when the weak_ptr is destroyed.
			//So we shouldn't have to worry about manually removing the subscriber before
			//it is destroyed.
			std::shared_ptr<std::function<void(Args ...)>> t(new std::function<void(Args ...)>(std::move(_subscriber)), [&](std::function<void(Args ...)>* obj) { delete obj; Cleanup(); });
			subscribers.push_back(t);
			return t;
		}

		void operator-=(std::shared_ptr<std::function<void(Args ...)>> _subscriber)
		{
			//this does not destroy the weak_ptr.
			//It will be destroyed at the next cleanup.
			//It means we may have a few weak_ptr hanging around until the next cleanup.
			_subscriber.reset();
		}

		void operator()(Args ... args)
		{
			auto subscribers_copy = subscribers;//prevent concurrent modification
			for (auto& wp_subscriber : subscribers)
			{
				if (auto sp_subscriber = wp_subscriber.lock())
				{
					(*sp_subscriber)(args ...);
				}
			}
		}

		void Cleanup()
		{
			for (auto& it = subscribers.begin(); it != subscribers.end();)
			{
				if (it->expired())
				{
					it = subscribers.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
	};
}