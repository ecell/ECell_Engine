#pragma once

//partial definitions:
namespace ECellEngine::Core
{
	class Engine;
}

#include "IO/CommandWithReceiver.hpp"
#include "Core/SimulationsManager.hpp"

namespace ECellEngine::IO
{
#pragma region IO Commands
	/*
	@brief The command to let the user set a path to a target file.
	*/
	/*class SetFilePathCommand : public CommandWithReceiver<FileIOManager>
	{
	public:
		SetFilePathCommand(FileIOManager* _receiver) :
			CommandWithReceiver("setFilePath", _receiver)
		{
		}

		void execute(const std::vector<std::string>& _args) override;
	};*/
#pragma endregion

#pragma region Engine Commands
	/*
	@brief The command to try to load a file as an SBML file.
	@details Uses the @a targetPathFile set with the other
			 command @a SetFilePathCommand.
	*/
	/*class AddFileAsSBMLCommand : public CommandWithReceiver<Engine>
	{
	public:
		AddFileAsSBMLCommand(Engine* _receiver) :
			CommandWithReceiver("addFileAsSBML", _receiver)
		{
		}

		void execute(const std::vector<std::string>& _args) override;
	};*/

	/*
	@brief The command to add a loaded SBML file to the targets to simulate.
	*/
	/*class AddSimulationTargetCommand : public CommandWithReceiver<Engine>
	{
	public:
		AddSimulationTargetCommand(Engine* _receiver) :
			CommandWithReceiver("addSimulationTarget", _receiver)
		{
		}

		void execute(const std::vector<std::string>& _args) override;
	};*/

	/*
	@brief The command to quit the application.
	@details Effectively exits from every running thread and then terminates.
	*/
	/*class QuitCommand : public CommandWithReceiver<Engine>
	{
	public:
		QuitCommand(Engine* _receiver) :
			CommandWithReceiver("quit", _receiver)
		{
		}

		void execute(const std::vector<std::string>& _args) override;
	};*/

#pragma endregion


#pragma region Simulation Loop Commands
	///*
	//@brief The command to display the current state of the simulation.
	//*/
	//class DisplayCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	DisplayCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("display", _receiver)
	//	{
	//	}

	//	virtual void execute(const std::vector<std::string>& _args) override;
	//};

	///*
	//@brief The command to instruct the simulation to go backward.
	//*/
	//class GoBackwardCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	GoBackwardCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("goBackward", _receiver)
	//	{
	//	}

	//	virtual void execute(const std::vector<std::string>& _args) override;
	//};

	///*
	//@brief The command to instruct the simulation to go forward.
	//*/
	//class GoForwardCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	GoForwardCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("goForward", _receiver)
	//	{
	//	}

	//	virtual void execute(const std::vector<std::string>& _args) override;
	//};

	///*
	//@brief The command to pause the simulation.
	//*/
	//class PauseCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	PauseCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("pause", _receiver)
	//	{
	//	}

	//	virtual void execute(const std::vector<std::string>& _args);
	//};

	///*
	//@brief The command to start or resume the simulation.
	//*/
	//class PlayCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	PlayCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("play", _receiver)
	//	{
	//	}

	//	virtual void execute(const std::vector<std::string>& _args) override;
	//};

	///*
	//@brief The command to step forward once in the simulation.
	//*/
	//class StepForwardCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	StepForwardCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("stepForward", _receiver)
	//	{
	//	}

	//	/*
	//	@brief Public interface to execute the action/code associated to the command
	//			and its arguments.
	//	@param[in] _args The arguments of the command. By default _args[0] is the
	//				name of the command. So the actual arguments are store from index
	//				1 onward. In this case, we expect index 1 to be the time we want
	//				to simulate forward.
	//	*/
	//	virtual void execute(const std::vector<std::string>& _args) override;
	//};

	///*
	//@brief The command to step backward once in the simulation.
	//*/
	//class StepBackwardCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	StepBackwardCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("stepBackward", _receiver)
	//	{
	//	}

	//	/*
	//	@brief Public interface to execute the action/code associated to the command
	//			and its arguments.
	//	@param[in] _args The arguments of the command. By default _args[0] is the
	//				name of the command. So the actual arguments are store from index
	//				1 onward. In this case, we expect index 1 to be the time we want
	//				to simulate backward.
	//	*/
	//	virtual void execute(const std::vector<std::string>& _args) override;
	//};

	///*
	//@brief The command to stop the simulation.
	//*/
	//class StopCommand : public CommandWithReceiver<SimulationLoop>
	//{
	//public:
	//	StopCommand(SimulationLoop* _receiver) :
	//		CommandWithReceiver("stop", _receiver)
	//	{
	//	}

	//	virtual void execute(const std::vector<std::string>& _args) override;
	//};
#pragma endregion

#pragma region Simulation Manager Commands

	/*
	@brief The command to let the user add a module in a simulation.
	*/
	class AddModuleCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		AddModuleCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("addModule", _receiver)
		{
		}

		/*
		@brief Executes the code to add a module to a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("addModule"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the path as a string pointing to the module's file.
		*/
		bool execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user add a solver in a simulation.
	*/
	class AddSolverCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		AddSolverCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("addSolver", _receiver)
		{
		}

		/*
		@brief Executes the code to add a solver to a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("addSolver"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the name of the kind of solver we wish to add.
		*/
		bool execute(const std::vector<std::string>& _args) override;
	};
	
	/*
	@brief The command to let the user pause a simulation.
	*/
	class PauseSimulationCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		PauseSimulationCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("pauseSimulation", _receiver)
		{
		}

		/*
		@brief Executes the code to pause a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("pauseSimulation"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				playing simulation list.
		*/
		bool execute(const std::vector<std::string>& _args) override;
	};
	
	/*
	@brief The command to let the user start (play) a simulation.
	*/
	class PlaySimulationCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		PlaySimulationCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("playSimulation", _receiver)
		{
		}

		/*
		@brief Executes the code to play a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("playSimulation"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list.
		*/
		bool execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user run one step of a simulation backward.
	*/
	class StepSimulationBackwardCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		StepSimulationBackwardCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("stepSimulationBackward", _receiver)
		{
		}

		/*
		@brief Executes the code to run one step of a simulation backward.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("stepSimulationBackward"). Then, for this command,
				come [1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the delta time as a float of the step we wish to take.
		*/
		bool execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user run one step of a simulation forward.
	*/
	class StepSimulationForwardCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		StepSimulationForwardCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("stepSimulationForward", _receiver)
		{
		}

		/*
		@brief Executes the code to run one step of a simulation forward.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("stepSimulationForward"). Then, for this command,
				come [1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] the delta time as a float of the step we wish to take.
		*/
		bool execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user stop a simulation.
	*/
	class StopSimulationCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		StopSimulationCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("stopSimulation", _receiver)
		{
		}

		/*
		@brief Executes the code to play a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("stopSimulation"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				playing simulation list.
		*/
		bool execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user bind a solver to a module in a simulation.
	*/
	class TryAttachSolverToModuleCommand : public CommandWithReceiver<ECellEngine::Core::SimulationsManager>
	{
	public:
		TryAttachSolverToModuleCommand(ECellEngine::Core::SimulationsManager* _receiver) :
			CommandWithReceiver("tryAttachSolver", _receiver)
		{
		}

		/*
		@brief Executes the code to play a simulation.
		@param _args The arguments of the command. At position [0] is always the
				name of the command ("tryAttachSolver"). Then, for this command, come
				[1] the index of the target simulation in the SimulationsManager's
				simulation list; [2] is the name of the solver in the Simulation's
				solvers list; [3] is the name of the module in the Simulation's
				modules list.

		*/
		bool execute(const std::vector<std::string>& _args) override;
	};

#pragma endregion

}