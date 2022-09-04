#include "Engine.hpp"

void Engine::forwardSimulationTarget(const int& _idx)
{
	simulationLoop.SetSimulationEnvironment(loadedSBMLDocuments[_idx]);
}

void Engine::start()
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

	std::cout << "Engine start" << std::endl;

	commandsManager.start();

	//Engine Commands
	commandsManager.registerCommand(std::make_shared<AddFileAsSBMLCommand>(this));
	commandsManager.registerCommand(std::make_shared<AddSimulationTargetCommand>(this));
	commandsManager.registerCommand(std::make_shared<QuitCommand>(this));	

	//IO Commands
	commandsManager.registerCommand(std::make_shared<SetFilePathCommand>(&fileIOManager));

	//Simulation Loop commands
	commandsManager.registerCommand(std::make_shared<DisplayCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<GoBackwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<GoForwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<PauseCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<PlayCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<StopCommand>(&simulationLoop));

	simulationLoop.start();

	//editor.start();

	isRunning = true;
}

void Engine::stop()
{
	simulationLoop.stop();

	isRunning = false;
}

void Engine::update()
{
	simulationLoop.update();
}
