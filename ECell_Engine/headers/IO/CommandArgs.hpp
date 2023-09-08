#pragma once

namespace ECellEngine::IO
{
	struct AddModuleCommandArgs
	{
		unsigned long long simulationID;
		char* modulePath;
		char* moduleName;
	};

	struct AddSolverCommandArgs
	{
		unsigned long long simulationID;
		char* solverName;
	};

	struct ModuleSolverLinkCommandArgs
	{
		unsigned long long simulationID;
		unsigned long long moduleID;
		unsigned long long solverID;

		ModuleSolverLinkCommandArgs() = default;

		ModuleSolverLinkCommandArgs(unsigned long long _simulationID, unsigned long long _moduleID, unsigned long long _solverID) :
			simulationID(_simulationID), moduleID(_moduleID), solverID(_solverID)
		{
		}
	};

	struct SimulationCommandArgs
	{
		unsigned long long simulationID;
	};

	struct StepSimulationCommandArgs
	{
		unsigned long long simulationID;
		float deltaTime;
	};
}