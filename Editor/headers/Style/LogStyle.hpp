#pragma once

#include <imgui.h>

#include "Logging/LogLevels.hpp"

/*!
@file LogStyle.hpp
@brief The file contains style for the logging system.
*/

namespace ECellEngine::Editor::Style
{
	/*!
	@brief The structure to store style for the logging system.
	*/
	struct LogStyle
	{
		/*!
		@brief The array to store all colors for the logging system.
		*/
		ImVec4 loggingColors[ECellEngine::Logging::LogLevel_Count];

		LogStyle()
		{
			loggingColors[ECellEngine::Logging::LogLevel_Debug] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			loggingColors[ECellEngine::Logging::LogLevel_Error] = ImVec4(0.90234375f, 0.296875f, 0.234375f, 1.0f);//dark (-ish) red HEX #e74c3c RGB (231, 76, 60) HSL (6 , 78 %, 57 %)
			loggingColors[ECellEngine::Logging::LogLevel_Trace] = ImVec4(0.3632825f, 0.42578125f, 0.4921825f, 1.0f);//Dark duck blue HEX #5d6d7e RGB (93, 109, 126) HSL (210, 15 %, 43 %)
			loggingColors[ECellEngine::Logging::LogLevel_Warning] = ImVec4(0.94140625f, 0.765625f, 0.05859375f, 1.0f);//dark (-ish) yellow HEX #f1c40f RGB (241, 196, 15) HSL (48, 89 %, 50 %)
		}
	};
}