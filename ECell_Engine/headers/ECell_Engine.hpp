#pragma once

#include <iostream>
#include <thread>

//partial definitions:
class SimulationLoop;
class KeyboardInput;

#include "simulation_loop.hpp"
#include "input_manager.hpp"
#include "commands.hpp"

class ECellEngine
{
public:
	EngineCommands engineCommands;

	SimulationLoop* simulationLoop;
	KeyboardInput* keyboardInput;

	bool isRunning = true;

	ECellEngine() : engineCommands(this)
	{

	}

	void Main();

	void SetKeyboardInput(KeyboardInput* _KI)
	{
		keyboardInput = _KI;
	}

	void SetSimulationLoop(SimulationLoop* _SimuLoop)
	{
		simulationLoop = _SimuLoop;
	}
};
