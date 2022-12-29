#include "EditorLog.hpp"

std::unique_ptr<ECellEngine::Editor::Debug::EditorLog> ECellEngine::Editor::Debug::EditorLog::singleton = nullptr;

std::unique_ptr<ECellEngine::Editor::Debug::EditorLog>& ECellEngine::Editor::Debug::EditorLog::GetSingleton() noexcept
{
	if (singleton == nullptr)
	{
		singleton = std::make_unique<ECellEngine::Editor::Debug::EditorLog>();
	}
	return singleton;
}

void ECellEngine::Editor::Debug::EditorLog::Log(const std::string& _msg) const noexcept
{
	logWidget->Log(_msg);
}