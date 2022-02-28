#include "ECell_Engine.hpp"
#include "simulation_loop.hpp"

// Declaring accessors of SimulationLoop
Gillespie_NRM_R SimulationLoop::GetSimulationEnvironment()
{
	return this->gillespieSimulationEnv;
}

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

		//Update Subsystems
		//Play catch up on the Gillespie Simulation
		gillespieSimulationEnv.RunForward(simulationTimer.elapsedTime);

		float endTime = simulationTimer.ReadHighResTimer();
		simulationTimer.deltaTime = simulationTimer.GetDuration(beginTime, endTime);
		simulationTimer.CheckSimulationDeltaTime();
		simulationTimer.elapsedTime += simulationTimer.deltaTime;

		beginTime = endTime;		
	}
	std::cout << "Exiting simulation Loop" << std::endl;
}

//Declaring mutators of SimulationLoop

void SimulationLoop::SetSimulationEnvironment()
{
	gillespieSimulationEnv.Initializes(refEngine->GetActiveDocument());
}

void SimulationLoop::SetSimulationState(SimulationState _simulationState)
{
	this->simulationState = _simulationState;
}
