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
	/*SBMLDocument* sbmlDoc = 0;
	bool SBMLok = false;
	sbmlDoc = sbmlWriter.GibsonAndBruckToyModel();
	SBMLok = sbmlParser.ValidateSBML(sbmlDoc);
	if (SBMLok) sbmlWriter.WriteSBML(sbmlDoc, "GibsonAndBruckToyModel.xml");
	delete sbmlDoc;
	std::cout << "Engine start" << std::endl;*/

	commandsManager.start();

	//Engine Commands
	commandsManager.registerCommand(std::make_shared<QuitCommand>(this));

	//IO Commands
	commandsManager.registerCommand(std::make_shared<OpenCommand>(this));
	//commandsManager.registerCommand(std::make_shared<LoadCommand>(this));

	//Simulation Loop commands
	commandsManager.registerCommand(std::make_shared<DisplayCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<GoBackwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<GoForwardCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<PauseCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<PlayCommand>(&simulationLoop));
	commandsManager.registerCommand(std::make_shared<StopCommand>(&simulationLoop));

	simulationLoop.start();
}

void Engine::stop()
{
	simulationLoop.stop();
	commandsManager.stop();
}

void  Engine::update()
{
	std::cout << "Engine update" << std::endl;

	std::thread sLThread{ &SimulationLoop::update, &simulationLoop };
	std::thread kIThread{ &CommandsManager::update, &commandsManager };

	sLThread.join();
	kIThread.join();

	std::cout << "All threads terminated." << std::endl;
}
