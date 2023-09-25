#include "Core/Engine.hpp"

void ECellEngine::Core::Engine::Start()
{
	//std::cout << "Engine start" << std::endl;
	ECellEngine::Logging::Logger::LogTrace("Engine Start");

	//SimulationManager Commands
	SimulationsManager& simulationManager = SimulationsManager::GetSingleton();
	commandsManager.RegisterCommand(std::make_shared<IO::AddModuleCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::AddSimulationCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::AddSolverCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::ClearDataStateCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::EraseAllDataOfTypeCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::EraseSimulationCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::ModuleSolverConnectionCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::ModuleSolverDisconnectionCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::PauseSimulationCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::PlaySimulationCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::StepSimulationBackwardCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::StepSimulationForwardCommand>(simulationManager));
	commandsManager.RegisterCommand(std::make_shared<IO::StopSimulationCommand>(simulationManager));

	//Creates a new simulation by default.
	simulationManager.AddSimulation();

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
