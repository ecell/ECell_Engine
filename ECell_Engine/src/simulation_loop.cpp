#include "simulation_loop.hpp"

// Declaring accessors of SimulationLoop
SimulationState SimulationLoop::GetSimulationState()
{
	return this->simulationState;
}

// Declaring logic of SimulationLoop
void SimulationLoop::LoopLogic()
{
	while (true)
	{
		while (simulationState != SimulationState::isPlaying)
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

		beginTime = endTime;		
	}
	std::cout << "Exiting simulation Loop" << std::endl;
}

//Declaring mutators of SimulationLoop
void SimulationLoop::SetSimulationState(SimulationState _simulationState)
{
	this->simulationState = _simulationState;
}
