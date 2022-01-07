#include "commands_center.hpp"

void CommandsCenter::Pointsman(std::string _command)
{
	if (strcmp(_command.c_str(), "pause") == 0)
	{
		simulationLoopCommands.pauseCommand.Execute();
	}

	else if (strcmp(_command.c_str(), "play") == 0)
	{
		simulationLoopCommands.playCommand.Execute();
	}

	else if (strcmp(_command.c_str(), "stop") == 0)
	{
		simulationLoopCommands.stopCommand.Execute();
	}

	else
	{
		std::cout << "The command:" << _command << " was not recognized" << std::endl;
	}
}