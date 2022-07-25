/*
	The main entrance of the simulation engine.
	Contains the simulation loop.
*/

#include "ECell_Engine.hpp"

void  ECellEngine::Main()
{
	SBMLDocument* sbmlDoc = 0;
	bool SBMLok = false;
	sbmlDoc = sbmlWriter.GibsonAndBruckToyModel();
	SBMLok = sbmlParser.ValidateSBML(sbmlDoc);
	if (SBMLok) sbmlWriter.WriteSBML(sbmlDoc, "GibsonAndBruckToyModel.xml");
	delete sbmlDoc;

	/*SBMLDocument* sbmlDoc = sbmlParser.OpenSBMLFile("GibsonAndBruckToyModel.xml");
	sbmlParser.PrettyPrintSBMLDocument(sbmlDoc);*/

	std::thread sLThread{ &SimulationLoop::LoopLogic, simulationLoop };
	std::thread kIThread{ &KeyboardInput::Start, keyboardInput };

	sLThread.join();
	kIThread.join();
}

void ECellEngine::OpenFile(const std::string* _filePath)
{
	std::cout << "Trying to open file: " << *_filePath << std::endl;
	SBMLDocument* sbmlDoc = sbmlParser.OpenSBMLFile((*_filePath).c_str());
	//activeDocument = sbmlParser.OpenSBMLFile((*_filePath).c_str());

	sbmlParser.PrettyPrintSBMLDocument(sbmlDoc);
	//simulationLoop->SetSimulationEnvironment(sbmlDoc);
	SetActiveSBMLDocument(sbmlDoc);

}
