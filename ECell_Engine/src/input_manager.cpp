#include "input_manager.hpp"

void KeyboardInput::Start()
{
	CommandsCenter cc(refSimulationLoop);

	while (true)
	{
		std::cin >> command;
		cc.Pointsman(command);
	};
}