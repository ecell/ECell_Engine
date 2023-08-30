#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "IO/Commands.hpp"
#include "Logging/Logger.hpp"

namespace ECellEngine::IO
{
	/*
	@brief Handles registration of commands, listening to keyboard string inputs
			and firing the execution of matching commands.
	*/
	class CommandsManager
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<AbstractCommand>> commands;

	public:
		CommandsManager() = default;

		/*
		@brief Searches for a command matching @p _cmdSplit and calls executes
				it if found.
		@param _cmdSplit The command content. The first element should be the
					command's name.
		@return True if the interpretation and execution of the command succeeds.
				False, otherwise.
		*/
		bool InterpretCommand(std::vector<std::string> const& _cmdSplit);

		/*
		@brief Adds @p _command to the register.
		@param _command The command to try to register.
		@returns False if the command already exists. True otherwise (and the command
				 is duely registered).
		*/
		bool RegisterCommand(std::shared_ptr<AbstractCommand> _command);

		/*
		@brief  Searches in the hash table of registered commands whether one has a name
				that matches @p _commandName.
		@param _commandName Target name of the command we are looking for.
		@returns The command encapsulated as a std::shared_ptr if a match was found. A
				 nullptr otherwise.
		*/
		std::shared_ptr<AbstractCommand> TryGetRegisteredCommand(std::string const& _commandName);
	};
}