/*
	The main entrance of the simulation engine.
	Contains the simulation loop.
*/

#include "launcher.hpp"

int main()
{
	ECellEngine engine;

	SimulationLoop simulationLoop(&engine);
	engine.SetSimulationLoop(&simulationLoop);

	KeyboardInput keyboardInput(&engine);
	keyboardInput.SetSystemCommands(&engine.engineCommands, &simulationLoop.simulationLoopCommands);
	
	engine.SetKeyboardInput(&keyboardInput);

	engine.Main();

	return 0;
}
