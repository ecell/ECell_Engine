#pragma once

#include <iostream>
#include <string>
#include <thread>

//partial definitions:
class SimulationLoop;
class KeyboardInput;

#include "simulation_loop.hpp"
#include "input_manager.hpp"
#include "commands.hpp"
#include "SBML_parsing.hpp"
#include "SBML_writing.hpp"

class ECellEngine
{
private:
	SBML_Writer sbmlWriter;
	SBML_Parser sbmlParser;
	SBMLDocument* activeDocument;

public:
	EngineCommands engineCommands;
	IOCommands ioCommands;

	SimulationLoop* simulationLoop;
	KeyboardInput* keyboardInput;

	bool isRunning = true;

	ECellEngine() : engineCommands(this), ioCommands(this)
	{

	}

#pragma region Accessors
	/// <summary>
	/// Gets the <see cref="activeDocument"/> private member.
	/// </summary>
	SBMLDocument* GetActiveDocument()
	{
		return activeDocument;
	}
#pragma endregion

#pragma region Mutators
	/// <summary>
	/// Sets the <see cref="activeDocument"/> private pointer member.
	/// </summary>
	void SetActiveSBMLDocument(SBMLDocument* _sbmlDoc)
	{
		activeDocument = _sbmlDoc;
	}

	/// <summary>
	/// Sets the <see cref="keyboardInput"/> public pointer member.
	/// </summary>
	void SetKeyboardInput(KeyboardInput* _KI)
	{
		keyboardInput = _KI;
	}

	/// <summary>
	/// Sets the <see cref="simulationLoop"/> public pointer member.
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

	/// <summary>
	/// The main entry to sorting
	/// </summary>
	void OpenFile(const std::string* _filePath);
#pragma endregion

};
