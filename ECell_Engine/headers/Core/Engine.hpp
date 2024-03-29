﻿#pragma once

#include <iostream>
#include <string>
#include <thread>

#include "IO/CommandsManager.hpp"
#include "Logging/Logger.hpp"

using namespace ECellEngine::IO;

namespace ECellEngine::Core
{
	/*!
	@brief The heart of ECellEngine.
	@details Controls everything related to initializing, starting up,
		 updating and ending the modules of the engine.
	*/
	class Engine
	{
		/*!
		@brief The manager to call specific tasks from outside the engine's code.
		*/
		CommandsManager commandsManager;

	public:
		/*
		@brief True is ::Start() was called and ::Stop() has no yet been called.
		*/
		bool isRunning = false;
		
		Engine() = default;

#pragma region Accessors

		/*!
		@brief Gets the pointer to ::commandsManager private member.
		*/
		inline CommandsManager* GetCommandsManager()
		{
			return &commandsManager;
		}

#pragma endregion

#pragma region Mutators
#pragma endregion

#pragma region Logic
		/*!
		@brief Initializes everything needed for the engine to start running.
		@details Registers commands.
		*/
		void Start();

		/*!
		@brief Stops any running simulation and cleans up everything.
		*/
		void Stop();

		/*!
		@brief Update the simulations via the ::simulationManager.
		*/
		void Update(float _deltaTime);
#pragma endregion

	};
}