/*
	The main entrance of the simulation engine.
	Contains the simulation loop.
*/

#include "ECell_Engine.hpp"

void  ECellEngine::Main()
{
	SBML_Writer sbmlWriter;
	SBML_Parser sbmlParser;

	SBMLDocument* sbmlDoc = 0;
	bool SBMLok = false;
	sbmlDoc = sbmlWriter.GibsonAndBruckToyModel();
	SBMLok = sbmlParser.ValidateSBML(sbmlDoc);
	if (SBMLok) sbmlWriter.WriteSBML(sbmlDoc, "GibsonAndBruckToyModel.xml");
	delete sbmlDoc;



	std::thread sLThread{ &SimulationLoop::LoopLogic, simulationLoop };
	std::thread kIThread{ &KeyboardInput::Start, keyboardInput };

	sLThread.join();
	kIThread.join();


}
