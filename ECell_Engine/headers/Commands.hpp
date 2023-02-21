#pragma once

//partial definitions:
namespace ECellEngine::Core
{
	class Engine;
	class Simulation;
}
#include "CommandWithReceiver.hpp"

using namespace ECellEngine::Core;

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

#pragma region Simulation Commands

	/*
	@brief The command to let the user add a module in a simulation.
	*/
	class AddModuleCommand : public CommandWithReceiver<Simulation>
	{
	public:
		AddModuleCommand(Simulation* _receiver) :
			CommandWithReceiver("addModule", _receiver)
		{
		}

		void execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user add a solver in a simulation.
	*/
	class AddSolverCommand : public CommandWithReceiver<Simulation>
	{
	public:
		AddSolverCommand(Simulation* _receiver) :
			CommandWithReceiver("addSolver", _receiver)
		{
		}

		void execute(const std::vector<std::string>& _args) override;
	};

	/*
	@brief The command to let the user bind a solver to a module in a simulation.
	*/
	class TryAttachSolverToModuleCommand : public CommandWithReceiver<Simulation>
	{
	public:
		TryAttachSolverToModuleCommand(Simulation* _receiver) :
			CommandWithReceiver("tryAttachSolver", _receiver)
		{
		}

		void execute(const std::vector<std::string>& _args) override;
	};

#pragma endregion

}