#pragma once 

#include "Logger.hpp"
#include "LogWidget.hpp"

namespace ECellEngine::Editor::Debug
{
	class EditorLog : public ECellEngine::Debug::Logger
	{
	private:
		ECellEngine::Editor::LogWidget* logWidget;
		static std::unique_ptr<EditorLog> singleton;

	public:
		static EditorLog& GetSingleton() noexcept;

		inline void SetLogWidget(ECellEngine::Editor::LogWidget* _logWidget)
		{
			logWidget = _logWidget;
		}

		inline void Log(const std::string& _msg) const noexcept override
		{
			logWidget->Log(_msg);
		}
	};
}