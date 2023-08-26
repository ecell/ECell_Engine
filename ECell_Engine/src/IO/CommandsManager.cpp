#include "IO/CommandsManager.hpp"

bool ECellEngine::IO::CommandsManager::interpretCommand(std::vector<std::string> const& _cmdSplit)
{
	std::shared_ptr<Command> matchingCommand = tryGetRegisteredCommand(_cmdSplit[0]);
	std::string msg = "Command: ";
	if (matchingCommand.get() != nullptr)
	{
		//Execute the command
		for (int i = 0; i<_cmdSplit.size(); i++)
		{
			msg += " " + _cmdSplit[i];
		}
		ECellEngine::Logging::Logger::LogTrace(msg.c_str());
		return matchingCommand->execute(_cmdSplit);
	}
	else
	{
		msg += _cmdSplit[0] +  " doesn't exist!";
		ECellEngine::Logging::Logger::LogError(msg.c_str());
		return false;
	}
}

bool ECellEngine::IO::CommandsManager::registerCommand(std::shared_ptr<Command> _command)
{
	return commands.emplace(_command->getName(), _command).second;
}

std::shared_ptr<ECellEngine::IO::Command> ECellEngine::IO::CommandsManager::tryGetRegisteredCommand(std::string const& commandName)
{
	auto foundCommandIterator = commands.find(commandName);

	return (foundCommandIterator != commands.cend()) ? foundCommandIterator->second : nullptr;
}