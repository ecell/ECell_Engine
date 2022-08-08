#include "simulation_loop.hpp"

void SimulationLoop::start()
{
	std::cout << "SimulationLoop start" << std::endl;
	isRunning = true;
}

void SimulationLoop::stop()
{
	std::cout << "SimulationLoop stop" << std::endl;
	isRunning = false;
}


void SimulationLoop::update()
{
	std::cout << "SimulationLoop update; isRunning = " << isRunning << std::endl;
	while (isRunning)
	{
		while(simulationState != SimulationState::isPlaying && isRunning)
		{
			std::this_thread::yield();
		}

		float beginTime = simulationTimer.ReadHighResTimer();

		//Update Subsystems
		//Play catch up on the Gillespie Simulation
		gillespieSimulationEnv.RunForward(simulationTimer.elapsedTime);

		//
		//DO OTHER THINGS REALTED TO THE ENGINE
		//

		//Compute delta time
		float endTime = simulationTimer.ReadHighResTimer();
		simulationTimer.deltaTime = simulationTimer.GetDuration(beginTime, endTime);
		simulationTimer.CheckSimulationDeltaTime();
		simulationTimer.elapsedTime += simulationTimer.deltaTime;

		beginTime = endTime;		
	}
	std::cout << "Exiting simulation Loop" << std::endl;
}
