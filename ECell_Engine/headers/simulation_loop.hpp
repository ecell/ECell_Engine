#pragma once

#include <iostream>
#include <thread>

#include "system_time.hpp"
#include "simulation_timer.hpp"


enum SimulationState { isPaused, isPlaying, isStopped };

class SimulationLoop
{
private:
	//float nextConsoleDisplayRefreshTime;
	SimulationState simulationState = SimulationState::isStopped;

public:
	Timer simulationTimer;

	//Accessors
	SimulationState GetSimulationState();

	//Class logic
	void LoopLogic();

	//Mutators
	void SetSimulationState(SimulationState _simulationState);

	/*SimulationLoop()
	{
		nextConsoleDisplayRefreshTime = CONSOLE_DISPLAY_REFRESH_RATE;
	};*/

};


