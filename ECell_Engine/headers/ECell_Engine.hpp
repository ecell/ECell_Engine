#pragma once

#include <iostream>
#include <thread>

//partial definitions:
class SimulationLoop;
class KeyboardInput;

#include "simulation_loop.hpp"
#include "input_manager.hpp"
#include "commands.hpp"
#include "sbml_example.hpp"

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
#pragma region Mutators
	/// <summary>
	/// Set the <see cref="keyboardInput"/> public pointer member.
	/// </summary>
	void SetKeyboardInput(KeyboardInput* _KI)
	{
		keyboardInput = _KI;
	}

	/// <summary>
	/// Set the <see cref="simulationLoop"/> public pointer member.
	/// </summary>
	void SetSimulationLoop(SimulationLoop* _SimuLoop)
	{
		simulationLoop = _SimuLoop;
	}
#pragma endregion

#pragma region Logic
	/// <summary>
	/// The method to start up the engine.
	/// </summary>
	void Main();
#pragma endregion

};
