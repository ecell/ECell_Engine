#include "CommandsManager.hpp"

void ECellEngine::IO::CommandsManager::interpretCommand(std::vector<std::string> const& _cmdSplit)
{
	std::shared_ptr<Command> matchingCommand = tryGetRegisteredCommand(_cmdSplit[0]);

	if (matchingCommand.get() != nullptr)
	{
		//Execute the command
		std::cout << "Executing: ";
		for (int i = 0; i<_cmdSplit.size(); i++)
		{
			std::cout << _cmdSplit[i] << " | ";
		}
		std::cout << std::endl;
		matchingCommand->execute(_cmdSplit);
	}
	else
	{
		std::cout << "The command \"" << _cmdSplit[0] << "\" doesn't exist!" << std::endl;
	}
}

bool ECellEngine::IO::CommandsManager::registerCommand(std::shared_ptr<Command> _command)
{
	return commands.emplace(_command->getName(), _command).second;
}

void ECellEngine::IO::CommandsManager::start()
{
	std::cout << "CommandsManager start" << std::endl;
	isListening = true;
}

void ECellEngine::IO::CommandsManager::stop()
{
	std::cout << "CommandsManager stop" << std::endl;
	isListening = false;
}

std::shared_ptr<ECellEngine::IO::Command> ECellEngine::IO::CommandsManager::tryGetRegisteredCommand(std::string const& commandName)
{
	auto foundCommandIterator = commands.find(commandName);

	return (foundCommandIterator != commands.cend()) ? foundCommandIterator->second : nullptr;
}

void ECellEngine::IO::CommandsManager::update()
{
	std::string commandInput;
	std::vector<std::string> commandInputSplit;

	std::cout << "CommandsManager update" << std::endl;
	while (isListening)
	{
		commandInput = waitForConsoleInput();
		commandInputSplit = splitStr(commandInput);
		interpretCommand(commandInputSplit);
	};
	std::cout << "Exiting keyboard input polling loop" << std::endl;
}