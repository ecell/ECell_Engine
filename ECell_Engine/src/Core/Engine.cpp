#include "Core/Engine.hpp"

void ECellEngine::Core::Engine::Start()
{
	//std::cout << "Engine start" << std::endl;
	ECellEngine::Logging::Logger::LogTrace("Engine Start");

	//SimulationManager Commands
	SimulationsManager& simulationManager = SimulationsManager::GetSingleton();
	commandsManager.RegisterCommand(std::make_shared<AddModuleCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<AddSolverCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<ModuleSolverConnectionCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<PauseSimulationCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<PlaySimulationCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<StepSimulationBackwardCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<StepSimulationForwardCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<StopSimulationCommand>(simulationManager));

	//Creates a new simulation by default.
	simulationManager.NewSimulation();

	isRunning = true;
}

void ECellEngine::Core::Engine::Stop()
{
	isRunning = false;
}

void ECellEngine::Core::Engine::Update(float _deltaTime)
{
	SimulationsManager::GetSingleton().UpdatePlayingSimulations(_deltaTime);
}
