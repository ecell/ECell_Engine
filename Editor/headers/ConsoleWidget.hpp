#pragma once

#include <string>
#include <memory>

#include "EditorConsoleLoggerSink.hpp"
#include "Logger.hpp"
#include "Widget.hpp"

using namespace ECellEngine::Editor::Logging;

namespace ECellEngine::Editor
{
	
	/*! \file ConsoleWidget.hpp */

	/*!
	@brief An enum to handle different types of log messages.
	*/
	enum LogLevel {
		debug, /*!< Intended to be used to print messages during development.*/
		error, /*!< Reports errors to the user of the engine.*/
		trace, /*!< Reports operation performed by the engine to the user.*/
		warning /*!< Reports warnings to the user.*/
	};

	/*!
	@brief The struct containing the data used to correctly 
		display a message in the console of the editor.
	*/
	struct LogMessage
	{
		std::string msg;
		ImVec4 color;

		LogMessage(const LogLevel _lvl, const std::string& _msg) :
			msg{_msg}
		{
			ProcessLevel(_lvl);
		}

		/*!
		@brief Formats the message depending on its #ECellEngine::Editor::LogLevel
		*/
		void ProcessLevel(const LogLevel _lvl)
		{
			switch (_lvl)
			{
			case(LogLevel::debug):
				color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);//white
				break;
			case(LogLevel::error):
				color = ImVec4(0.90234375f, 0.296875f, 0.234375f, 1.0f);//dark (-ish) red HEX #e74c3c RGB (231, 76, 60) HSL (6 , 78 %, 57 %)
				break;
			case(LogLevel::trace):
				color = ImVec4(0.3632825f, 0.42578125f, 0.4921825f, 1.0f);//Dark duck blue HEX #5d6d7e RGB (93, 109, 126) HSL (210, 15 %, 43 %)
				break;
			case(LogLevel::warning):
				color = ImVec4(0.94140625f, 0.765625f, 0.05859375f, 1.0f);//dark (-ish) yellow HEX #f1c40f RGB (241, 196, 15) HSL (48, 89 %, 50 %)
				break;
			}
		}
	};

	/*!
	@brief The window displaying a console where messages about the state of
			the simulation are printed.
	*/
	class ConsoleWidget : public Widget
	{
	
	private:
		EditorConsoleLoggerSink ecLoggerSink; /*!< The logger sink used to collect the messages.*/
		std::vector<LogMessage> log; /*< The collection of log messages received.*/

	public:
		ConsoleWidget(Editor& _editor) :
			Widget(_editor), ecLoggerSink(*this)
		{
			ecLoggerSink.sinkIdx = ECellEngine::Logging::Logger::GetSingleton().AddSink(&ecLoggerSink);
		}

		~ConsoleWidget()
		{
			ECellEngine::Logging::Logger::GetSingleton().RemoveSink(ecLoggerSink.sinkIdx);
		}

		inline void Awake() override {};

		void Draw() override;

		/**
		* @brief Display a message of type #ECellEngine::Editor::LogLevel::trace
		*		 in this ConsoleWidget of the Editor.
		*/
		inline void LogTrace(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::trace, _msg));
		}
		
		/**
		* @brief Display a message of type #ECellEngine::Editor::LogLevel::debug
		*		 in this ConsoleWidget of the Editor.
		*/
		inline void LogDebug(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::debug, _msg));
		}
		
		/**
		* @brief Display a message of type #ECellEngine::Editor::LogLevel::error
		*		 in this ConsoleWidget of the Editor.
		*/
		inline void LogError(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::error, _msg));
		}
		
		/**
		* @brief Display a message of type #ECellEngine::Editor::LogLevel::warning
		*		 in this ConsoleWidget of the Editor.
		*/
		inline void LogWarning(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::warning, _msg));
		}
	};
}