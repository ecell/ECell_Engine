#pragma once

/*!
@file CommandArgs.h
@brief Defines convenience data structures to be used as arguments for commands.
@see ECellEngine::IO::Command
*/

namespace ECellEngine::IO
{
	struct EmptyCommandArgs
	{

	};

	struct AddModuleCommandArgs
	{
		unsigned long long simulationID = 0;
		char* modulePath = nullptr;
		char* moduleName = nullptr;
	};

	struct AddSolverCommandArgs
	{
		unsigned long long simulationID = 0;
		char* solverName;
	};

	struct ModuleSolverLinkCommandArgs
	{
		unsigned long long simulationID = 0;
		unsigned long long moduleID = 0;
		unsigned long long solverID = 0;

		ModuleSolverLinkCommandArgs() = default;

		ModuleSolverLinkCommandArgs(unsigned long long _simulationID, unsigned long long _moduleID, unsigned long long _solverID) :
			simulationID(_simulationID), moduleID(_moduleID), solverID(_solverID)
		{
		}
	};

	struct SimulationCommandArgs
	{
		unsigned long long simulationID = 0;
	};

	struct EraseAllDataOfTypeCommandArgs
	{
		unsigned long long simulationID = 0;
		char* dataType = nullptr;
	};

	struct StepSimulationCommandArgs
	{
		unsigned long long simulationID = 0;
		float deltaTime = 0.01f;
	};
}