#include "ECell_Engine.hpp"
#include "input_manager.hpp"

void KeyboardInput::Pointsman()
{

#pragma region Main Application Commands
	if (strcmp(command.c_str(), "quit") == 0)
	{
		engineCommands->quitCommand.Execute();
	}
#pragma endregion

#pragma region Simulation Loop Commands
	else if (strcmp(command.c_str(), "display") == 0)
	{
		simulationLoopCommands->displayCommand.Execute();
	}

	else if (strcmp(command.c_str(), "pause") == 0)
	{
		simulationLoopCommands->pauseCommand.Execute();
	}

	else if (strcmp(command.c_str(), "play") == 0)
	{
		simulationLoopCommands->playCommand.Execute();
	}

	else if (strcmp(command.c_str(), "stop") == 0)
	{
		simulationLoopCommands->stopCommand.Execute();
	}
#pragma endregion

	else
	{
		std::cout << "The command:" << command << " was not recognized" << std::endl;
	}
}

void KeyboardInput::SetSystemCommands(EngineCommands* _refMAC, SimulationLoopCommands* _refSLC)
{
	engineCommands = _refMAC;
	simulationLoopCommands = _refSLC;
}

void KeyboardInput::Start()
{
	while (refEngine->isRunning)
	{
		std::cin >> command;
		Pointsman();
	};
	std::cout << "Exiting keyboard input polling loop" << std::endl;
}