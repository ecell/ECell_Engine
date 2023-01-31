#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Commands.hpp"
#include "Logger.hpp"

namespace ECellEngine::IO
{
	/*
	@brief Handles registration of commands, listening to keyboard string inputs
			and firing the execution of matching commands.
	*/
	class CommandsManager
	{
	private:
		bool isListening;
		std::unordered_map<std::string, std::shared_ptr<Command>> commands;

		/*
		@brief Splits a string according to white spaces
		@param _str The string we want to split.
		@returns A vector containing the token of the splitted string.
		*/
		inline std::vector<std::string> splitStr(std::string _str)
		{
			std::istringstream iss(_str);
			std::vector<std::string> results(std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>());
			return results;
		}

		/*
		@brief Blocks the execution of the current thread to and gets
				a string input from the console.
		@returns The string input.
		*/
		inline std::string waitForConsoleInput()
		{
			std::string input;
			std::cin >> input;
			return input;
		}

	public:
		CommandsManager() = default;

		/*
		@brief Searches for a command matching @p _cmdSplit and calls executes
				it if found.
		@param _cmdSplit The command content. The first element should be the
					command's name.
		*/
		void interpretCommand(std::vector<std::string> const& _cmdSplit);

		/*
		@brief Adds @p _command to the register.
		@param _command The command to try to register.
		@returns False if the command already exists. True otherwise (and the command
				 is duely registered).
		*/
		bool registerCommand(std::shared_ptr<Command> _command);

		/*
		@brief Does everything needed to start listening to commands.
		*/
		void start();

		/*
		@brief Does everything needed to stop listening to commands.
		*/
		void stop();

		/*
		@brief  Searches in the hash table of registered commands whether one has a name
				that matches @p _commandName.
		@param _commandName Target name of the command we are looking for.
		@returns The command encapsulated as a std::shared_ptr if a match was found. A
				 nullptr otherwise.
		*/
		std::shared_ptr<Command> tryGetRegisteredCommand(std::string const& _commandName);

		/*
		@brief Update loop to listen to commands and process them.
		*/
		void update();
	};
}