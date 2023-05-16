#include "Core/Engine.hpp"

void ECellEngine::Core::Engine::Start()
{
	/*SBMLDocument* sbmlDoc = sbmlParser.OpenSBMLFile("GibsonAndBruckToyModel.xml");
	sbmlParser.PrettyPrintSBMLDocument(sbmlDoc);*/
	
	//Writes toy model for tests.
	/*
	SBMLDocument* sbmlDoc = 0;
	bool SBMLok = false;
	sbmlDoc = sbmlWriter.GibsonAndBruckToyModel();
	SBMLok = sbmlParser.ValidateSBML(sbmlDoc);
	if (SBMLok) sbmlWriter.WriteSBML(sbmlDoc, "GibsonAndBruckToyModel.xml");
	delete sbmlDoc;
	*/

	//std::cout << "Engine start" << std::endl;
	ECellEngine::Logging::Logger::GetSingleton().LogTrace("Engine Start");

	commandsManager.start();

	//Engine Commands
	//commandsManager.registerCommand(std::make_shared<QuitCommand>(this));

	//SimulationManager Commands
	commandsManager.registerCommand(std::make_shared<AddModuleCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<AddSolverCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<PauseSimulationCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<PlaySimulationCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<StepSimulationBackwardCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<StepSimulationForwardCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<StopSimulationCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<TryAttachSolverToModuleCommand>(&simulationManager));

	//Creates a new simulation by default.
	simulationManager.NewSimulation();

	isRunning = true;
}

void ECellEngine::Core::Engine::Stop()
{
	//simulation.stop();

	isRunning = false;
}

void ECellEngine::Core::Engine::Update(float _deltaTime)
{
	simulationManager.UpdatePlayingSimulations(_deltaTime);
}
