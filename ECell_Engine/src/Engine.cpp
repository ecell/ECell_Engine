#include "Engine.hpp"

void ECellEngine::Core::Engine::start()
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
	//commandsManager.registerCommand(std::make_shared<AddFileAsSBMLCommand>(this));
	//commandsManager.registerCommand(std::make_shared<AddSimulationTargetCommand>(this));
	//commandsManager.registerCommand(std::make_shared<QuitCommand>(this));	

	//IO Commands
	//commandsManager.registerCommand(std::make_shared<SetFilePathCommand>(&fileIOManager));

	//Simulation Loop commands
	/*
	commandsManager.registerCommand(std::make_shared<DisplayCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<GoBackwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<GoForwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<PauseCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<PlayCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<StepBackwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<StepForwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<StopCommand>(&simulationLoop));
	*/

	//SimulationManager Commands
	commandsManager.registerCommand(std::make_shared<AddModuleCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<AddSolverCommand>(&simulationManager));
	commandsManager.registerCommand(std::make_shared<TryAttachSolverToModuleCommand>(&simulationManager));

	//editor.start();

	isRunning = true;
}

void ECellEngine::Core::Engine::stop()
{
	//simulation.stop();

	isRunning = false;
}

void ECellEngine::Core::Engine::update(float _deltaTime)
{
	//simulation.update(_deltaTime);
}
