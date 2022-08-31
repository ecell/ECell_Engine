#pragma once

#include "imgui.h"

namespace ECellEngine::Editor
{
	class Windows
	{
	public:

		static void Options(bool* _showDemoWindow);

		/*
		@brief  Builds the window with the buttons
				  to control the flow of the simulation.
		@details  We find the start, pause, stop, forward
				  & backward buttons.
		*/
		static void SimulationFlowControl();
	};
}

