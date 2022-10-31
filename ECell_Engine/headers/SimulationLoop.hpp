#pragma once

#include <atomic>
#include <iostream>
#include <thread>

#include "system_time.hpp"
#include "Timer.hpp"
#include "Gillespie_NRM_R.hpp"


enum SimulationState { isPaused, isPlaying, isStopped };

class SimulationLoop
{
private:
	SimulationState simulationState = SimulationState::isStopped;
	Gillespie_NRM_R gillespieSimulationEnv;

	bool isRunning = false;
	short direction = 1;

public:
	SimulationLoop() {}
	virtual ~SimulationLoop() {}

	Timer simulationTimer;

#pragma region Accessors
	
	/*
	@brief Gets the direction private member.
	*/
	inline short* getSimulationDirection()
	{
		return &direction;
	}
	
	/*
	@brief Gets the gillespieSimulationEnv private member.
	*/
	inline Gillespie_NRM_R* getSimulationEnvironment()
	{
		return &gillespieSimulationEnv;
	}

	/*
	@brief Gets the simulationState private member.
	*/
	inline SimulationState* getSimulationState()
	{
		return &simulationState;
	}
#pragma endregion

#pragma region Mutators
	/*
	@brief Sets the direction private member to -1.
	*/
	inline void setSimulationDirectionToBackward()
	{
		direction = -1;
	}
	
	/*
	@brief Sets the direction private member to 1.
	*/
	inline void setSimulationDirectionToForward()
	{
		direction = 1;
	}

	/*
	@brief Initialize the simulation environment gillespieSimulationEnv with 
			@p _refModel.
	@param[in] _refModel A model described in the SBML format.
	*/
	inline void SetSimulationEnvironment(SBMLDocument* _refModel)
	{
		gillespieSimulationEnv.Initializes(_refModel);
	}

	/*
	@brief Sets the simulationState private member.
	*/
	inline void SetSimulationState(SimulationState _simulationState)
	{
		simulationState = _simulationState;
	}
	
#pragma endregion

#pragma region Logic

	/*
	@brief Does everything needed to start up the simulation loop.
	*/
	void start();

	/*
	@brief Does everything needed to stop the simulation loop.
	*/
	void stop();

	/*
	@brief Does everything to compute one step backward the simulation
	@param _deltaTime The time lenght of the step
	*/
	void stepBackward(const float _deltaTime);

	/*
	@brief Does everything to compute one step forward the simulation
	*/
	void stepForward(const float _deltaTime);

	/*
	@brief	 The core of the simulation loop.
	@details Calls every simulation's subsystem to be updated.
			 Typically called in a new thread when the ECellEngine
			 is starting up.
	*/
	void update(const float _deltaTime);
#pragma endregion

};


