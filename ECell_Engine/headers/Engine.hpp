#pragma once

#include <iostream>
#include <string>
#include <thread>

#include "SimulationLoop.hpp"
#include "CommandsManager.hpp"
#include "Commands.hpp"
#include "Editor.hpp"
#include "SbmlParser.hpp"
#include "SbmlWriter.hpp"

/*
@brief The main class of the engine.
@details Controls everything related to initializing, starting up,
		 updating and ending the modules of the engine.
*/
class Engine
{
private:
	SbmlWriter sbmlWriter;
	SbmlParser sbmlParser;
	SBMLDocument* activeDocument;

	CommandsManager commandsManager;
	ECellEngine::Editor::Editor editor;
	SimulationLoop simulationLoop;
	

public:
	Engine() = default;

	bool isRunning;

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
