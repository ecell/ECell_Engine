#pragma once

//partial definitions:
namespace ECellEngine::Core
{
	class Engine;
}

#include "IO/Command.hpp"
#include "Core/SimulationsManager.hpp"

namespace ECellEngine::IO
{
#pragma region Simulation Manager Commands

	/*
	@brief The command to let the user add a module in a simulation.
	*/
	class AddModuleCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		AddModuleCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("addModule"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to add a module to a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("addModule"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the path as a string pointing to the module's file.
		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user add a solver in a simulation.
	*/
	class AddSolverCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		AddSolverCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("addSolver"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to add a solver to a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("addSolver"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the name of the kind of solver we wish to add.
		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};
	
	/*
	@brief The command to let the user pause a simulation.
	*/
	class PauseSimulationCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		PauseSimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("pauseSimulation"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to pause a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("pauseSimulation"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				playing simulation list.
		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};
	
	/*
	@brief The command to let the user start (play) a simulation.
	*/
	class PlaySimulationCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		PlaySimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("playSimulation"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to play a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("playSimulation"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list.
		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user run one step of a simulation backward.
	*/
	class StepSimulationBackwardCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		StepSimulationBackwardCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("stepSimulationBackward"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to run one step of a simulation backward.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("stepSimulationBackward"). Then, for this command,
				come [1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the delta time as a float of the step we wish to take.
		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user run one step of a simulation forward.
	*/
	class StepSimulationForwardCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		StepSimulationForwardCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("stepSimulationForward"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to run one step of a simulation forward.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("stepSimulationForward"). Then, for this command,
				come [1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the delta time as a float of the step we wish to take.
		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user stop a simulation.
	*/
	class StopSimulationCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		StopSimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("stopSimulation"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to play a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("stopSimulation"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				playing simulation list.
		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user bind a solver to a module in a simulation.
	*/
	class TryModuleSolverLinkCommand final : public Command
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		TryModuleSolverLinkCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("tryLinkModuleWithSolver"), receiver(_receiver)
		{
		}

		/*
		@brief Executes the code to play a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("tryLinkModuleWithSolver"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] is the name of the solver in the Simulation's
				solvers list; [3] is the name of the module in the Simulation's
				modules list.

		*/
		bool Execute(const std::vector<std::string>& _args) override;
	};

#pragma endregion

}