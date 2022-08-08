#pragma once

#include <iostream>
#include <string>
#include <thread>

#include "simulation_loop.hpp"
#include "CommandsManager.hpp"
#include "commands.hpp"
#include "SBML_parsing.hpp"
#include "SBML_writing.hpp"

/*
@brief The main class of the engine.
@details Controls everything related to initializing, starting up,
		 updating and ending the modules of the engine.
*/
class ECellEngine
{
private:
	SBML_Writer sbmlWriter;
	SBML_Parser sbmlParser;
	SBMLDocument* activeDocument;

	SimulationLoop simulationLoop;
	CommandsManager commandsManager;

public:
	ECellEngine() = default;

	bool isRunning = true;

#pragma region Accessors
	/*
	@brief Gets the @a activeDocument private member.
	*/
	inline SBMLDocument* GetActiveDocument()
	{
		return activeDocument;
	}
#pragma endregion

#pragma region Mutators
	/*
	@brief Sets the @a activeDocument private member.
	*/
	inline void SetActiveSBMLDocument(SBMLDocument* _sbmlDoc)
	{
		activeDocument = _sbmlDoc;
	}
#pragma endregion

#pragma region Logic	
	/*
	@brief The main entry to sorting.
	*/
	void OpenFile(const std::string* _filePath);

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
