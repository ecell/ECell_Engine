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