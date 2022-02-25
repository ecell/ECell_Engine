/*
	The main entrance of the simulation engine.
	Contains the simulation loop.
*/

#include "ECell_Engine.hpp"

void  ECellEngine::Main()
{
	SBMLDocument* sbmlDoc = 0;
	bool SBMLok = false;
	sbmlDoc = createExampleEnzymaticReaction_();
	SBMLok = validateExampleSBML_(sbmlDoc);
	if (SBMLok) writeExampleSBML_(sbmlDoc, "enzymaticreaction.xml");
	delete sbmlDoc;

	std::thread sLThread{ &SimulationLoop::LoopLogic, simulationLoop };
	std::thread kIThread{ &KeyboardInput::Start, keyboardInput };

	sLThread.join();
	kIThread.join();


}
