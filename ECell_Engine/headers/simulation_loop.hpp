#pragma once

#include <iostream>
#include <thread>

//partial definitions:
class ECellEngine;

#include "commands.hpp"
#include "system_time.hpp"
#include "simulation_timer.hpp"
#include "Gillespie_NRM.hpp"


enum SimulationState { isPaused, isPlaying, isStopped };

class SimulationLoop
{
private:
	ECellEngine* refEngine;
	SimulationState simulationState = SimulationState::isStopped;
	Gillespie_NRM_R gillespieSimulationEnv;
	
public:
	Timer simulationTimer;
	
	SimulationLoopCommands simulationLoopCommands;

	SimulationLoop(ECellEngine* _refEngine) :
		refEngine(_refEngine),
		simulationLoopCommands(this)
	{

	}

#pragma region Accessors
	/// <summary>
	/// Get the <see cref="gillespieSimulationEnv"/>
	/// </summary>
	Gillespie_NRM_R GetSimulationEnvironment();

	/// <summary>
	/// Get the <see cref="simulationState"/> private member.
	/// </summary>
	SimulationState GetSimulationState();
#pragma endregion

#pragma region Mutators
	/// <summary>
	/// Uses the current active SBML Document in the ECell Engine to initialize
	/// the simulation environment <see cref="gillespieSimulationEnv"/>.
	/// </summary>
	void SetSimulationEnvironment();

	/// <summary>
	/// Set the <see cref="simulationState"/> private member.
	/// </summary>
	void SetSimulationState(SimulationState _simulationState);
	
#pragma endregion

#pragma region Logic
	/// <summary>
	/// The core of the simulation loop. Calls every simulation's
	/// subsystem to be updated.
	/// Typically called in a new thread when the ECellEngine
	/// is starting up.
	/// </summary>
	void LoopLogic();
#pragma endregion

};


