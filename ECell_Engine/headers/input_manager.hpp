#pragma once

//partial definitions
class ECellEngine;

#include "string.h"
#include "commands.hpp"
//#include "ECell_Engine.hpp"

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

	void Pointsman();

	void SetSystemCommands(EngineCommands*, SimulationLoopCommands*);

	void Start();
};