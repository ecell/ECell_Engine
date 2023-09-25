#pragma once

/*!
@file Command.hpp
@brief All the commands used in the engine.
*/

//Forward declaration
namespace ECellEngine::Core
{
	class Engine;
}

#include "IO/Command.hpp"
#include "IO/CommandArgs.hpp"
#include "Core/SimulationsManager.hpp"

namespace ECellEngine::IO
{
#pragma region Simulation Manager Commands

	/*
	@brief The command to let the user add a module in a simulation.
	@details At position [0] is always the name of the command ("addModule").
			 Then, for this command, come [1] the ID of the target
			 simulation in the SimulationsManager's simulation list; [2] the
			 path as a string pointing to the module's file; [3] the name of
			 the module.
	*/
	class AddModuleCommand final : public Command<AddModuleCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		AddModuleCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("addModule", 4), receiver(_receiver)
		{
			args.simulationID = 0;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Add a module to a simulation.\n"
					"Usage: addModule <simulationID> <modulePath> <moduleName>\n"
					"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.\n"
					"modulePath: the path as a string pointing to the module's file.\n"
					"moduleName: the name of the module.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to add a module to a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user add a new simulation.
	@details At position [0] is the name of the command ("addSimulation").
			 This command has no parameters.
	*/
	class AddSimulationCommand final : public Command<EmptyCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		AddSimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("addSimulation", 1), receiver(_receiver)
		{
			
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Add a simulation to the engine.\n"
					"Usage: addSimulation <NO_ARGUMENTS>\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to add a module to a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const EmptyCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user add a solver in a simulation.
	@details At position [0] is always the name of the command ("addSolver").
			 Then, for this command, come [1] the ID of the target
			 simulation in the SimulationsManager's simulation list; [2] the
			 name of the kind of solver we wish to add.
	*/
	class AddSolverCommand final : public Command<AddSolverCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		AddSolverCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("addSolver", 3), receiver(_receiver)
		{
			args.simulationID = 0;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Add a solver to a simulation.\n"
				"Usage: addSolver <simulationID> <solverName>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.\n"
				"solverName: the name of the kind of solver we wish to add (GillespieNRMRSolver or GeneralizedExplicitRK)";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to add a solver to a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user clear the datastate of a simulation.
	@details At position [0] is always the name of the command ("clearDataState").
			 Then, for this command, come [1] the ID of the target
			 simulation in the SimulationsManager's simulation list.
	*/
	class ClearDataStateCommand final : public Command<SimulationCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		ClearDataStateCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("clearDataState", 2), receiver(_receiver)
		{
			
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Clears the datastate of a simulation.\n"
				"Usage: clearDataState <simulationID>\n"
				"simulationID: the ID of the target simulation for which we want to clear the data state.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to erase a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const SimulationCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user clear a container (erase all data inside)
			in the datastate of a simulation.
	@details At position [0] is always the name of the command ("eraseAllDataOfType").
			 Then, for this command, come [1] the ID of the target
			 simulation in the SimulationsManager's simulation list; [2] is the
			 string describing which data container to clear.
	*/
	class EraseAllDataOfTypeCommand final : public Command<EraseAllDataOfTypeCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		EraseAllDataOfTypeCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("eraseAllDataOfType", 3), receiver(_receiver)
		{
			
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Clears a target container identified by its contained data type in the datastate of a simulation.\n"
				"Usage: eraseAllDataOfType <simulationID> <dataType>\n"
				"simulationID: The ID of the target simulation for which we want to clear the data state.\n"
				"dataType: The string describing which data container to clear.\n"
				"			Allowed values are: Equation, Parameter, Reaction, Species, OperandsToOperation,\n"
				"			Operation, LogicOperation, ModifyDataStateValueEvent, Trigger\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to clear a container (erase all data inside) in
				the datastate of a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const EraseAllDataOfTypeCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user erase one element from a container in
				the datastate of a simulation.
	@details At position [0] is always the name of the command ("eraseDataOfType").
			 Then, for this command, come [1] the ID of the target
			 simulation in the SimulationsManager's simulation list; [2] is the
			 string describing which data container to clear; [3] is the ID of
			 the element in the datastate.
	*/
	class EraseDataOfTypeCommand final : public Command<EraseDataOfTypeCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		EraseDataOfTypeCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("eraseDataOfType", 4), receiver(_receiver)
		{
			
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Erases a target element of a container identified by its contained data type in the datastate of a simulation.\n"
				"Usage: eraseDataOfType <simulationID> <dataType> <dataID>\n"
				"simulationID: The ID of the target simulation for which we want to clear the data state.\n"
				"dataType: The string describing which data container to clear.\n"
				"			Allowed values are: Equation, Parameter, Reaction, Species, OperandsToOperation,\n"
				"			Operation, LogicOperation, ModifyDataStateValueEvent, Trigger\n"
				"dataID: The ID of the element in the datastate.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to erase one element from a container in
				the datastate of a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const EraseDataOfTypeCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user erase a simulation.
	@details At position [0] is always the name of the command ("eraseSimulation").
			 Then, for this command, come [1] the ID of the target
			 simulation in the SimulationsManager's simulation list.
	*/
	class EraseSimulationCommand final : public Command<SimulationCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		EraseSimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("eraseSimulation", 2), receiver(_receiver)
		{
			args.simulationID = 0;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Erases a simulation.\n"
				"Usage: eraseSimulation <simulationID>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to erase a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const SimulationCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user bind a solver and a module in a simulation.
	@details At position [0] is always the name of the command ("moduleSolverConnection").
			 Then, for this command, come [1] the ID of the target simulation in the
			 SimulationsManager's simulation list; [2] is the ID of the module in the
			 Simulation's modules list; [3] is the ID of the solver in the Simulation's
			 solvers list.
	*/
	class ModuleSolverConnectionCommand final : public Command<ModuleSolverLinkCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		ModuleSolverConnectionCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("moduleSolverConnection", 4), receiver(_receiver)
		{
			args.simulationID = 0;
			args.moduleID = 0;
			args.solverID = 0;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Connect a module to a solver in a simulation.\n"
				"Usage: moduleSolverConnection <simulationID> <moduleID> <solverID>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.\n"
				"moduleID: the ID of the module in the Simulation's modules list.\n"
				"solverID: the ID of the solver in the Simulation's solvers list.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to connection a module and solver in a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user unbind a solver and a module in a simulation.
	@details At position [0] is always the name of the command ("moduleSolverDisconnection").
			 Then, for this command, come [1] the ID of the target simulation in the
			 SimulationsManager's simulation list; [2] is the ID of the module in the
			 Simulation's modules list; [3] is the ID of the solver in the Simulation's
			 solvers list.
	*/
	class ModuleSolverDisconnectionCommand final : public Command<ModuleSolverLinkCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		ModuleSolverDisconnectionCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("moduleSolverDisconnection", 4), receiver(_receiver)
		{
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Disconnect a module from a solver in a simulation.\n"
				"Usage: moduleSolverDisconnection <simulationID> <moduleID> <solverID>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.\n"
				"moduleID: the ID of the module in the Simulation's modules list.\n"
				"solverID: the ID of the solver in the Simulation's solvers list.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to disconnect a module and a solver in a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user pause a simulation.
	@details At position [0] is always the name of the command ("pauseSimulation").
			 Then, for this command, come [1] the ID of the target simulation in the
			 SimulationsManager's simulation list.
	*/
	class PauseSimulationCommand final : public Command<SimulationCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		PauseSimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("pauseSimulation", 2), receiver(_receiver)
		{
			args.simulationID = 0;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Pause a simulation.\n"
				"Usage: pauseSimulation <simulationID>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to pause a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user start (play) a simulation.
	@details At position [0] is always the name of the command ("startSimulation").
			 Then, for this command, come [1] the ID of the target simulation in the
			 SimulationsManager's simulation list.
	*/
	class PlaySimulationCommand final : public Command<SimulationCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		PlaySimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("playSimulation", 2), receiver(_receiver)
		{
			args.simulationID = 0;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Start (play) a simulation.\n"
				"Usage: startSimulation <simulationID>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to play a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user run one step of a simulation backward.
	@details At position [0] is always the name of the command ("stepSimulationBackward").
			 Then, for this command, come [1] the ID of the target simulation in the
			 SimulationsManager's simulation list; [2] the delta time as a float of the
			 step we wish to take.
	*/
	class StepSimulationBackwardCommand final : public Command<StepSimulationCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		StepSimulationBackwardCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("stepSimulationBackward", 3), receiver(_receiver)
		{
			args.simulationID = 0;
			args.deltaTime = 0.01;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Run one step of a simulation backward.\n"
				"Usage: stepSimulationBackward <simulationID> <deltaTime>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.\n"
				"deltaTime: the delta time as a float of the step we wish to take.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to run one step of a simulation backward.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user run one step of a simulation forward.
	@details At position [0] is always the name of the command ("stepSimulationForward").
			 Then, for this command, come [1] the ID of the target simulation in the
			 SimulationsManager's simulation list; [2] the delta time as a float of the
			 step we wish to take.
	*/
	class StepSimulationForwardCommand final : public Command<StepSimulationCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		StepSimulationForwardCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("stepSimulationForward", 3), receiver(_receiver)
		{
			args.simulationID = 0;
			args.deltaTime = 0.01;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Run one step of a simulation forward.\n"
				"Usage: stepSimulationForward <simulationID> <deltaTime>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.\n"
				"deltaTime: the delta time as a float of the step we wish to take.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to run one step of a simulation forward.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*
	@brief The command to let the user stop a simulation.
	@details At position [0] is always the name of the command ("stopSimulation").
			 Then, for this command, come [1] the ID of the target simulation in the
			 SimulationsManager's playing simulation list.
	*/
	class StopSimulationCommand final : public Command<SimulationCommandArgs>
	{
		ECellEngine::Core::SimulationsManager& receiver;

	public:
		StopSimulationCommand(ECellEngine::Core::SimulationsManager& _receiver) :
			Command("stopSimulation", 2), receiver(_receiver)
		{
			args.simulationID = 0;
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Stop a simulation.\n"
				"Usage: stopSimulation <simulationID>\n"
				"simulationID: the ID of the target simulation in the SimulationsManager's simulation list.";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to play a simulation.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddModuleCommandArgs& _params).
		*/
		bool Execute() override;
	};

#pragma endregion

}