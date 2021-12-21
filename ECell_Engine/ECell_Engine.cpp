/*
	The main entrance of the simulation engine.
	Contains the simulation loop.
*/

#include "ECell_Engine.hpp"

int main()
{
	SimulationLoop simulationLoop;
	std::string command;

	while (true)
	{
		std::cin >> command;
		simulationLoop.LoopCommandCenter(command);
	}

	//std::cout << "next_console_refresh_time:" << next_console_refresh_time << std::endl;

	/*while (main_loop_timer.readHighResTimer() < 10.0)
	{
		if (main_loop_timer.readHighResTimer() > next_console_refresh_time)
		{
			next_console_refresh_time += CONSOLE_DISPLAY_REFRESH_RATE;
			std::printf("Currently, %.2f seconds has passed\r", main_loop_timer.readHighResTimer());
			std::cout << std::flush;
		}
	}
	std::cout << std::endl;*/
	return 0;
}
