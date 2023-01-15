#pragma once

#include <iostream>
#include <string>
#include <thread>

#include "Commands.hpp"
#include "CommandsManager.hpp"
#include "Logger.hpp"
#include "Simulation.hpp"

using namespace ECellEngine::IO;

namespace ECellEngine::Core
{
/*
@brief The main class of the engine.
@details Controls everything related to initializing, starting up,
		 updating and ending the modules of the engine.
*/
	class Engine
	{
		CommandsManager commandsManager;
		Simulation simulation;

	public:
		Engine() = default;

		bool isRunning;

#pragma region Accessors

		/*
		@brief Gets the pointer to @a commandsManager private member.
		*/
		inline CommandsManager* GetCommandsManager()
		{
			return &commandsManager;
		}

		/*
		@brief Gets the pointer to @a simulationLoop private member.
		*/
		inline Simulation& GetSimulationLoop()
		{
			return simulation;
		}

#pragma endregion

#pragma region Mutators
#pragma endregion

#pragma region Logic
		/*
		@brief Initializes every sub modules or variable needed for the engine
				to be able to start running.
		*/
		void start();

		/*
		@brief Does everything needed to stop the engine.
		*/
		void stop();

		/*
		@brief The method to run the engine.
		*/
		void update(float _deltaTime);
#pragma endregion

	};
}