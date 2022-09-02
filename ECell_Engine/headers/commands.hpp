#pragma once

#include <iostream>

//partial definitions:
class Engine;
class SimulationLoop;
class FileIOManager;

#include "CommandWithReceiver.hpp"

#pragma region IO Commands
class SetFilePathCommand : public CommandWithReceiver<FileIOManager>
{
public:
	SetFilePathCommand(FileIOManager* _receiver):
		CommandWithReceiver("setFilePath", _receiver)
	{
	}

	void execute() override;
};
#pragma endregion

#pragma region Engine Commands
class AddFileAsSBMLCommand : public CommandWithReceiver<Engine>
{
public:
	AddFileAsSBMLCommand(Engine* _receiver) :
		CommandWithReceiver("addFileAsSBML", _receiver)
	{
	}

	void execute() override;
};
/*
@brief The command to quit the application.
@details Effectively exits from every running thread and then terminates.
*/
class QuitCommand : public CommandWithReceiver<Engine>
{
public:
	QuitCommand(Engine* _receiver):
		CommandWithReceiver("quit", _receiver)
	{
	}

	void execute() override;
};

#pragma endregion


#pragma region Simulation Loop Commands
/*
@brief The command to display the current state of the simulation.
*/
class DisplayCommand : public CommandWithReceiver<SimulationLoop>
{
public:
	DisplayCommand(SimulationLoop* _receiver) :
		CommandWithReceiver("display", _receiver)
	{
	}

	virtual void execute() override;
};

/*
@brief The command to instruct the simulation to go forward.
*/
class GoForwardCommand : public CommandWithReceiver<SimulationLoop>
{
public:
	GoForwardCommand(SimulationLoop* _receiver) :
		CommandWithReceiver("goforward", _receiver)
	{
	}

	virtual void execute() override;
};

/*
@brief The command to instruct the simulation to go backward.
*/
class GoBackwardCommand : public CommandWithReceiver<SimulationLoop>
{
public:
	GoBackwardCommand(SimulationLoop* _receiver) :
		CommandWithReceiver("gobackward", _receiver)
	{
	}

	virtual void execute() override;
};

/*
@brief The command to pause the simulation.
*/
class PauseCommand : public CommandWithReceiver<SimulationLoop>
{
public:
	PauseCommand(SimulationLoop* _receiver) :
		CommandWithReceiver("pause", _receiver)
	{
	}

	virtual void execute();
};

/*
@brief The command to start or resume the simulation.
*/
class PlayCommand : public CommandWithReceiver<SimulationLoop>
{
public:
	PlayCommand(SimulationLoop* _receiver) :
		CommandWithReceiver("play", _receiver)
	{
	}

	virtual void execute() override;
};

/*
@brief The command to stop the simulation.
*/
class StopCommand : public CommandWithReceiver<SimulationLoop>
{
public:
	StopCommand(SimulationLoop* _receiver) :
		CommandWithReceiver("stop", _receiver)
	{
	}

	virtual void execute() override;
};
#pragma endregion

