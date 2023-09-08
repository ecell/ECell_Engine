#include "IO/CommandsManager.hpp"

bool ECellEngine::IO::CommandsManager::ProcessCommand(std::vector<std::string> const& _cmdSplit)
{
	std::shared_ptr<AbstractCommand> matchingCommand = TryGetRegisteredCommand(_cmdSplit[0]);
	std::string msg = "Processing command: ";
	if (matchingCommand.get() != nullptr)
	{
		//Execute the command
		for (int i = 0; i<_cmdSplit.size(); i++)
		{
			msg += " " + _cmdSplit[i];
		}
		ECellEngine::Logging::Logger::LogTrace(msg.c_str());
		return matchingCommand->ProcessCommand(_cmdSplit);
	}
	else
	{
		msg += _cmdSplit[0] +  " doesn't exist!";
		ECellEngine::Logging::Logger::LogError(msg.c_str());
		return false;
	}
}

bool ECellEngine::IO::CommandsManager::RegisterCommand(std::shared_ptr<AbstractCommand> _command)
{
	return commands.emplace(_command->GetName(), _command).second;
}

std::shared_ptr<ECellEngine::IO::AbstractCommand> ECellEngine::IO::CommandsManager::TryGetRegisteredCommand(std::string const& commandName)
{
	auto foundCommandIterator = commands.find(commandName);

	return (foundCommandIterator != commands.cend()) ? foundCommandIterator->second : nullptr;
}