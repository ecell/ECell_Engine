#include "CommandsManager.hpp"

void ECellEngine::Editor::CommandsManager::processCommand(std::vector<std::string> const& _cmdSplit)
{
	std::shared_ptr<Command> matchingCommand = tryGetRegisteredCommand(_cmdSplit[0]);

	if (matchingCommand.get() != nullptr)
	{
		//Execute the command
		matchingCommand->execute();
	}
	else
	{
		std::cout << "The command \"" << _cmdSplit[0] << "\" doesn't exist!" << std::endl;
	}
}

bool ECellEngine::Editor::CommandsManager::registerCommand(std::shared_ptr<Command> _command)
{
	return commands.emplace(_command->getName(), _command).second;
}

std::shared_ptr<Command> ECellEngine::Editor::CommandsManager::tryGetRegisteredCommand(std::string const& commandName)
{
	auto foundCommandIterator = commands.find(commandName);

	return (foundCommandIterator != commands.cend()) ? foundCommandIterator->second : nullptr;
}