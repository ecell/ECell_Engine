#pragma once

#include <iostream>

class ECellEngine;
class SimulationLoop;

#include "command.hpp"

#pragma region Main Application Commands

class QuitCommand : Command
{
public:
	virtual void Execute();

	QuitCommand() {};
	QuitCommand(ECellEngine* _refEngine) : receiver(_refEngine) {};

private:
	ECellEngine* receiver;
};

struct EngineCommands
{
public:
	QuitCommand quitCommand;

	EngineCommands() {};

	EngineCommands(ECellEngine* _refEngine) :
		quitCommand(_refEngine)
	{

	};
};

#pragma endregion


#pragma region Simulation Loop Commands
class DisplayCommand : Command
{
public:
	virtual void Execute();

	DisplayCommand(){};
	DisplayCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

class PauseCommand : Command
{
public:
	virtual void Execute();

	PauseCommand(){};
	PauseCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

class PlayCommand : Command
{
public:
	virtual void Execute();

	PlayCommand(){};
	PlayCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

class StopCommand : Command
{
public:
	virtual void Execute();

	StopCommand(){};
	StopCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

struct SimulationLoopCommands
{
public:
	DisplayCommand displayCommand;
	PauseCommand pauseCommand;
	PlayCommand playCommand;
	StopCommand stopCommand;

	SimulationLoopCommands() {};

	SimulationLoopCommands(SimulationLoop* _refSimuLoop) :
		displayCommand(_refSimuLoop),
		pauseCommand(_refSimuLoop),
		playCommand(_refSimuLoop),
		stopCommand(_refSimuLoop)
	{

	};
};
#pragma endregion

