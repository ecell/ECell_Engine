#include "ECell_Engine.hpp"
#include "input_manager.hpp"

void KeyboardInput::Pointsman(std::vector<std::string> _cmdSplit)
{
#pragma region IO Commands
	if (_cmdSplit[0] == "open")
	{
		ioCommands->openCommand.Execute();
	}
#pragma endregion

#pragma region Engine Commands
	else if (_cmdSplit[0] == "quit")
	{
		engineCommands->quitCommand.Execute();
	}
#pragma endregion

#pragma region Simulation Loop Commands
	else if (_cmdSplit[0] == "display")
	{
		simulationLoopCommands->displayCommand.Execute();
	}

	else if (_cmdSplit[0] == "load")
	{
		simulationLoopCommands->loadCommand.Execute();
	}

	else if (_cmdSplit[0] == "pause")
	{
	simulationLoopCommands->pauseCommand.Execute();
	}

	else if (_cmdSplit[0] == "play")
	{
	simulationLoopCommands->playCommand.Execute();
	}

	else if (_cmdSplit[0] == "stop")
	{
		simulationLoopCommands->stopCommand.Execute();
	}
#pragma endregion

	else
	{
		std::cout << "The command:" << command << " was not recognized" << std::endl;
	}
}

void KeyboardInput::SetSystemCommands(
	EngineCommands* _refEC,
	IOCommands* _refIOC,
	SimulationLoopCommands* _refSLC)
{
	engineCommands = _refEC;
	ioCommands = _refIOC;
	simulationLoopCommands = _refSLC;
}

void KeyboardInput::Start()
{
	while (refEngine->isRunning)
	{
		std::cin >> command;
		std::istringstream iss(command);
		std::vector<std::string> results(std::istream_iterator<std::string>{iss},
										 std::istream_iterator<std::string>());

		Pointsman(results);
	};
	std::cout << "Exiting keyboard input polling loop" << std::endl;
}