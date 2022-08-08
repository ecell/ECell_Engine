#include "ECell_Engine.hpp"
#include "CommandsManager.hpp"

void CommandsManager::processCommand(std::vector<std::string> const& _cmdSplit)
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

bool CommandsManager::registerCommand(std::shared_ptr<Command> _command)
{
	return commands.emplace(_command->getName(), _command).second;
}

void CommandsManager::start()
{
	std::cout << "CommandsManager start" << std::endl;
	isListening = true;
}

void CommandsManager::stop()
{
	std::cout << "CommandsManager stop" << std::endl;
	isListening = false;
}

std::shared_ptr<Command> CommandsManager::tryGetRegisteredCommand(std::string const& commandName)
{
	auto foundCommandIterator = commands.find(commandName);

	return (foundCommandIterator != commands.cend()) ? foundCommandIterator->second : nullptr;
}

void CommandsManager::update()
{
	std::cout << "CommandsManager update" << std::endl;
	while (isListening)
	{
		std::cin >> commandInput;
		std::istringstream iss(commandInput);
		std::vector<std::string> results(std::istream_iterator<std::string>{iss},
			std::istream_iterator<std::string>());

		processCommand(results);
	};
	std::cout << "Exiting keyboard input polling loop" << std::endl;
}