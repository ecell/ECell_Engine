#pragma once

#include <iostream>
#include <thread>

//partial definitions:
class ECellEngine;

#include "commands.hpp"
#include "system_time.hpp"
#include "simulation_timer.hpp"


enum SimulationState { isPaused, isPlaying, isStopped };

class SimulationLoop
{
private:
	ECellEngine* refEngine;
	SimulationState simulationState = SimulationState::isStopped;

public:
	Timer simulationTimer;
	SimulationLoopCommands simulationLoopCommands;

	SimulationLoop(ECellEngine* _refEngine) :
		refEngine(_refEngine),
		simulationLoopCommands(this)
	{

	}

	//Accessors
	SimulationState GetSimulationState();

	//Class logic
	void LoopLogic();

	//Mutators
	void SetSimulationState(SimulationState _simulationState);

};


