#pragma once

/*!
@file LogLevel.h
@brief Declares the enum to describe log levels.
*/

namespace ECellEngine::Logging
{
	/*!
	@brief An enum to describe different types of log levels.
	*/
	enum LogLevel : unsigned char
	{
		LogLevel_Debug, /*!< Intended to be used to print messages during development.*/
		LogLevel_Error, /*!< Reports errors to the user of the engine.*/
		LogLevel_Trace, /*!< Reports operation performed by the engine to the user.*/
		LogLevel_Warning, /*!< Reports warnings to the user.*/

		LogLevel_Count /*!< The number of log levels.*/
	};
}