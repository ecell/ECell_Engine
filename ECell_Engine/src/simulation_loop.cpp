#include "ECell_Engine.hpp"
#include "simulation_loop.hpp"
#include "Gillespie_NRM.hpp"

// Declaring accessors of SimulationLoop
SimulationState SimulationLoop::GetSimulationState()
{
	return this->simulationState;
}

// Declaring logic of SimulationLoop
void SimulationLoop::LoopLogic()
{
	Gillespie_NRM_R WorldStateSimulator(7, 5, 123);
	//std::cout << WorldStateSimulator.imht

	while (refEngine->isRunning)
	{
		while(simulationState != SimulationState::isPlaying && refEngine->isRunning)
		{
			std::this_thread::yield();
		}

		float beginTime = simulationTimer.ReadHighResTimer();

		//Update Subsystems
		//Play catch up on the Gillespie Simulation
		WorldStateSimulator.RunForward(simulationTimer.elapsedTime);

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
