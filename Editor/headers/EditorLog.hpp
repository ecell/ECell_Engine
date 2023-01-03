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

		void Log(const char* _msg) const noexcept override;

	};
}