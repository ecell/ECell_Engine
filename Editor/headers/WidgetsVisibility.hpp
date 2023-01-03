#pragma once

#include <memory>

namespace ECellEngine::Editor
{
	class WidgetsVisibility
	{
	private:
		static std::unique_ptr<WidgetsVisibility> singleton;

		bool logWidget;

	public:

		WidgetsVisibility()
		{
			logWidget = false;
		}
		
		static WidgetsVisibility& GetSingleton() noexcept;

		inline bool* GetLogWidgetVisibility() noexcept
		{
			return &logWidget;
		};


	};
}
