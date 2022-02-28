#pragma once

#include <iostream>

//partial definitions:
class ECellEngine;
class SimulationLoop;

#include "command.hpp"

#pragma region IO Commands
class OpenCommand : Command
{
public:
	void Execute();

	OpenCommand() {};
	OpenCommand(ECellEngine* _refEngine) : receiver(_refEngine) {};

private:
	ECellEngine* receiver;
	std::string answer;

protected:
	virtual void AskUser();
};

struct IOCommands
{
public:
	OpenCommand openCommand;

	IOCommands() {};

	IOCommands(ECellEngine* _refEngine) :
		openCommand(_refEngine)
	{

	};
};
#pragma endregion

#pragma region Engine Commands

/// <summary>
/// The command to quit the application. Effectively
/// exits from every running thread and then terminates.
/// </summary>
class QuitCommand : Command
{
public:
	virtual void Execute();

	QuitCommand() {};
	QuitCommand(ECellEngine* _refEngine) : receiver(_refEngine) {};

private:
	ECellEngine* receiver;
};

/// <summary>
/// The struct encapsulating every command having a global
/// effect on the application.
/// </summary>
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
/// <summary>
/// The command to display the current state of the simulation.
/// </summary>
class DisplayCommand : Command
{
public:
	virtual void Execute();

	DisplayCommand(){};
	DisplayCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

/// <summary>
/// The command to load the current active model in the simulation environment.
/// </summary>
class LoadCommand : Command
{
public:
	virtual void Execute();

	LoadCommand() {};
	LoadCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

/// <summary>
/// The command to pause the simulation.
/// </summary>
class PauseCommand : Command
{
public:
	virtual void Execute();

	PauseCommand(){};
	PauseCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

/// <summary>
/// The command to start or resume the simulation.
/// </summary>
class PlayCommand : Command
{
public:
	virtual void Execute();

	PlayCommand(){};
	PlayCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

/// <summary>
/// The command to stop the simulation
/// </summary>
class StopCommand : Command
{
public:
	virtual void Execute();

	StopCommand(){};
	StopCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

/// <summary>
/// The struct encapsulating every command having an effect
/// on the simulation.
/// </summary>
struct SimulationLoopCommands
{
public:
	DisplayCommand displayCommand;
	LoadCommand loadCommand;
	PauseCommand pauseCommand;
	PlayCommand playCommand;
	StopCommand stopCommand;

	SimulationLoopCommands() {};

	SimulationLoopCommands(SimulationLoop* _refSimuLoop) :
		displayCommand(_refSimuLoop),
		loadCommand(_refSimuLoop),
		pauseCommand(_refSimuLoop),
		playCommand(_refSimuLoop),
		stopCommand(_refSimuLoop)
	{

	};
};
#pragma endregion

