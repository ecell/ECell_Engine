/*
	The main entrance of the simulation engine.
	Contains the simulation loop.
*/

#include "ECell_Engine.hpp"

int main()
{
	SimulationLoop simulationLoop;
	KeyboardInput keyboardInput(&simulationLoop);

	std::thread sLThread{ &SimulationLoop::LoopLogic, &simulationLoop };
	std::thread kIThread{ &KeyboardInput::Start, &keyboardInput };

	sLThread.join();
	kIThread.join();

	return 0;
}
