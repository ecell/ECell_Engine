#include "Engine.hpp"

void Engine::OpenFile(const std::string* _filePath)
{
	std::cout << "Trying to open file: " << *_filePath << std::endl;
	SBMLDocument* sbmlDoc = sbmlParser.OpenSBMLFile((*_filePath).c_str());

	sbmlParser.PrettyPrintSBMLDocument(sbmlDoc);
	SetActiveSBMLDocument(sbmlDoc);

	simulationLoop.SetSimulationEnvironment(sbmlDoc);
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

	//commandsManager.start();

	//Engine Commands
	//commandsManager.registerCommand(std::make_shared<QuitCommand>(this));

	//IO Commands
	//commandsManager.registerCommand(std::make_shared<OpenCommand>(this));
	//commandsManager.registerCommand(std::make_shared<LoadCommand>(this));

	//Simulation Loop commands
	//commandsManager.registerCommand(std::make_shared<DisplayCommand>(&simulationLoop));
	//commandsManager.registerCommand(std::make_shared<GoBackwardCommand>(&simulationLoop));
	//commandsManager.registerCommand(std::make_shared<GoForwardCommand>(&simulationLoop));
	//commandsManager.registerCommand(std::make_shared<PauseCommand>(&simulationLoop));
	//commandsManager.registerCommand(std::make_shared<PlayCommand>(&simulationLoop));
	//commandsManager.registerCommand(std::make_shared<StopCommand>(&simulationLoop));

	simulationLoop.start();

	//editor.start();

	isRunning = true;
}

void Engine::stop()
{
	simulationLoop.stop();
	//commandsManager.stop();
	//editor.stop();

	isRunning = false;
}

void Engine::update()
{
	simulationLoop.update();
	//std::thread commandsThread{ &CommandsManager::update, &commandsManager };
	//if (isRunning)
	//{
	//	simulationLoop.update();
	//	editor.update();
	//}
	////commandsThread.join();
}
