#include "launcher.hpp"

/// <summary>
/// Initializes the main subsystems. 
/// Links the subsystems to the engine.
/// Starts the engine.
/// </summary>
/// <returns>The 0 exit code.</returns>
int main()
{
	ECellEngine engine;

	SimulationLoop simulationLoop(&engine);
	engine.SetSimulationLoop(&simulationLoop);

	KeyboardInput keyboardInput(&engine);
	keyboardInput.SetSystemCommands(
		&engine.engineCommands,
		&engine.ioCommands,
		&simulationLoop.simulationLoopCommands);
	
	engine.SetKeyboardInput(&keyboardInput);

	engine.Main();

	return 0;
}
