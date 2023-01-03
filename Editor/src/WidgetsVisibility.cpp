#include "WidgetsVisibility.hpp"

std::unique_ptr<ECellEngine::Editor::WidgetsVisibility> ECellEngine::Editor::WidgetsVisibility::singleton = nullptr;

ECellEngine::Editor::WidgetsVisibility& ECellEngine::Editor::WidgetsVisibility::GetSingleton() noexcept
{
	if (singleton == nullptr)
	{
		singleton = std::make_unique<WidgetsVisibility>();
	}

	return *singleton.get();
}