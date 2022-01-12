#pragma once

//partial definitions
class ECellEngine;

#include "string.h"
#include "commands.hpp"

class KeyboardInput
{
private:
	EngineCommands* engineCommands;
	SimulationLoopCommands* simulationLoopCommands;

	ECellEngine* refEngine;

	std::string command;

public:
	KeyboardInput(ECellEngine* _refEngine) :
		refEngine(_refEngine)
	{
	};

#pragma region Mutators
	/// <summary>
	/// Set the pointers of every Commands struct declared in the
	/// engine and handled by the <see cref="input_manager"/>.
	/// Namely <see cref="engineCommands"/>, <see cref="simulationLoopCommands"/>.
	/// </summary>
	void SetSystemCommands(EngineCommands*, SimulationLoopCommands*);
#pragma endregion

#pragma region Logic
	/// <summary>
	/// The method processing the <see cref="command"/>.
	/// </summary>
	void Pointsman();

	/// <summary>
	/// The method starting up the input manager.
	/// Typically called in a new thread when the ECellEngine
	/// is starting up.
	/// </summary>
	void Start();
#pragma endregion

	

	
};