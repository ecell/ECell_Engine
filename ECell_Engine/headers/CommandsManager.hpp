#pragma once
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <memory>

#include "commands.hpp"

/*
@brief Handles registration of commands, listening to keyboard string inputs
		and firing the execution of matching commands.
*/
class CommandsManager
{
private:
	bool isListening;
	std::string commandInput;
	std::unordered_map<std::string, std::shared_ptr<Command>> commands;

public:
	CommandsManager() = default;

	/*
	@brief Searches for a command matching @p _cmdSplit and calls executes
			it if found.
	@param[in] _cmdSplit The command content. The first element should be the 
				command's name.
	*/
	void processCommand(std::vector<std::string> const& _cmdSplit);

	/*
	@brief Adds @p _command to the register.
	@param[in] _command The command to try to register.
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
	@param[in] _commandName Target name of the command we are looking for.
	@returns The command encapsulated as a std::shared_ptr if a match was found. A
			 nullptr otherwise.
	*/
	std::shared_ptr<Command> tryGetRegisteredCommand(std::string const& _commandName);

	/*
	@brief Update loop to listen to commands and process them.
	*/
	void update();
};