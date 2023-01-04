#pragma once

#include <memory>

namespace ECellEngine::Editor
{
	class WidgetsVisibility
	{
	private:
		static std::unique_ptr<WidgetsVisibility> singleton;

		bool logWidget;
		bool simulationWidget;

	public:
		
		static WidgetsVisibility& GetSingleton() noexcept;

		inline bool* GetLogWidgetVisibility() noexcept
		{
			return &logWidget;
		};
		
		inline bool* GetSimulationWidgetVisibility() noexcept
		{
			return &simulationWidget;
		};


	};
}
