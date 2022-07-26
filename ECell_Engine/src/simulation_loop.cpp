#include "ECell_Engine.hpp"
#include "simulation_loop.hpp"

// Declaring accessors of SimulationLoop
SimulationState SimulationLoop::GetSimulationState()
{
	return this->simulationState;
}

// Declaring logic of SimulationLoop
void SimulationLoop::LoopLogic()
{
	while (refEngine->isRunning)
	{
		while(simulationState != SimulationState::isPlaying && refEngine->isRunning)
		{
			std::this_thread::yield();
		}

		float beginTime = simulationTimer.ReadHighResTimer();

		//update subsystem 1
		//update subsystem 2
		//...
		//update subsystem N

		float endTime = simulationTimer.ReadHighResTimer();
		simulationTimer.deltaTime = simulationTimer.GetDuration(beginTime, endTime);
		simulationTimer.CheckSimulationDeltaTime();
		simulationTimer.elapsedTime += simulationTimer.deltaTime;

		beginTime = endTime;		
	}
	std::cout << "Exiting simulation Loop" << std::endl;
}

//Declaring mutators of SimulationLoop
void SimulationLoop::SetSimulationState(SimulationState _simulationState)
{
	this->simulationState = _simulationState;
}
