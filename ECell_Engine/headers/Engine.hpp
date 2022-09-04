#pragma once

#include <iostream>
#include <string>
#include <thread>

#include "SimulationLoop.hpp"
#include "FileIOManager.hpp"
#include "CommandsManager.hpp"
#include "Commands.hpp"

/*
@brief The main class of the engine.
@details Controls everything related to initializing, starting up,
		 updating and ending the modules of the engine.
*/
class Engine
{
	FileIOManager fileIOManager;
	CommandsManager commandsManager;
	SimulationLoop simulationLoop;
	
	std::vector<SBMLDocument*> loadedSBMLDocuments;

public:
	Engine() = default;

	bool isRunning;

#pragma region Accessors
	
	/*
	@brief Gets the pointer to @a commandsManager private member.
	*/
	inline CommandsManager* getCommandsManager()
	{
		return &commandsManager;
	}

	/*
	@brief Gets the pointer to @a fileIOManager private member.
	*/
	inline FileIOManager* getFileIOManager()
	{
		return &fileIOManager;
	}

	/*
	@brief Gets the pointer to @a loadedSBMLDocuments private member.
	*/
	inline std::vector<SBMLDocument*>* getLoadedSBMLDocuments()
	{
		return &loadedSBMLDocuments;
	}

	/*
	@brief Gets the @a sbmlDocument at index @a _idx in
			the @a loadedSBMLDocuments private member.
	*/
	inline SBMLDocument* getSBMLDocument(short _idx)
	{
		return loadedSBMLDocuments.at(_idx);
	}
	
	/*
	@brief Gets the pointer to @a simulationLoop private member.
	*/
	inline SimulationLoop* getSimulationLoop()
	{
		return &simulationLoop;
	}
	
#pragma endregion

#pragma region Mutators
	/*
	@brief Adds the @a _sbmlDoc to the @a loadedSBMLDocuments private member.
	*/
	inline void addSBMLDocument(SBMLDocument* _sbmlDoc)
	{
		loadedSBMLDocuments.push_back(_sbmlDoc);
	}
#pragma endregion

#pragma region Logic

	/*
	@brief Sends the SBMLDocument at index @a _idx in @a loadedSBMLDocuments
			to the simulation loop for simulation.
	*/
	void forwardSimulationTarget(const int& _idx);

	/*
	@brief Initializes every sub modules or variable needed for the engine
			to be able to start running.
	*/
	void start();

	/*
	@brief Does everything needed to stop the engine.
	*/
	void stop();

	/*
	@brief The method to run the engine.
	*/
	void update();
#pragma endregion

};
