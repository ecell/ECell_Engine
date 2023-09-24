#pragma once

/*!
@file Commands.hpp
@brief Contains the declarations of the commands that can be used in the editor.
*/

#include "IO/Command.hpp"
#include "IO/CommandArgsEditor.hpp"

//Forward declarations to avoid including too many headers just for a reference
namespace ECellEngine::Editor::Widget
{
	class ModelExplorerWidget;
}

namespace ECellEngine::Editor::IO
{
	/*!
	@brief Command to add a Model Node-Based Viewer Context under a simulation.
	*/
	class AddMNBVContextCommand : public ECellEngine::IO::Command<AddMNBVContextCommandArgs>
	{
		Widget::ModelExplorerWidget& receiver;

	public:
		AddMNBVContextCommand(Widget::ModelExplorerWidget& _receiver) :
			Command("addMNBVCtxt", 2), receiver(_receiver)
		{
			
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Add a model node-based viewer context under a simulation for the editor.\n"
				"Usage: addModule <Simulation ID>\n"
				"Simulation ID: The ID of the simulation to which to add the context.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to add a model node-based viewer under a simulation to access this
				simulation, view it, and add actions or data.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddMNBVContextCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*!
	@brief Command to erase all the nodes of a certain type in a target Model
			Node-Based Viewer Context.
	*/
	class EraseAllNodesOfTypeCommand : public ECellEngine::IO::Command<EraseAllNodesOfTypeCommandArgs>
	{
		Widget::ModelExplorerWidget& receiver;

	public:
		EraseAllNodesOfTypeCommand(Widget::ModelExplorerWidget& _receiver) :
			Command("eraseAllNodesOfType", 3), receiver(_receiver)
		{

		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Erases all the nodes of a certain type in a target model node-based viewer context.\n"
				"Usage: eraseAllNodesOfType <MNBVContext Index> <Node Type>\n"
				"MNBVContext Index: The index of the model node-based viewer context in the model explorer.\n"
				"Node Type: The type of the node to erase.\n"
				"			Authorized values are: Equation, Parameter, Reaction, Species, Arithmetic,\n"
				"			Asset, LinePlot, Logic, ModifyDataStateValueEvent, Solver, Time, Trigger,\n"
				"			ValueFloat.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to erase all nodes of a certain type in a target
				model node-based viewer context.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const EraseAllNodesOfTypeCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*!
	@brief Command to erase one node of a certain type, identified by it's ID,
			in a target Model Node-Based Viewer Context.
	*/
	class EraseNodeCommand : public ECellEngine::IO::Command<EraseNodeCommandArgs>
	{
		Widget::ModelExplorerWidget& receiver;

	public:
		EraseNodeCommand(Widget::ModelExplorerWidget& _receiver) :
			Command("eraseNode", 4), receiver(_receiver)
		{

		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Erases one node of a certain type identified by its ID in a target model node-based viewer context.\n"
				"Usage: eraseAllNodesOfType <MNBVContext Index> <Node Type> <Node ID>\n"
				"MNBVContext Index: The index of the model node-based viewer context in the model explorer.\n"
				"Node Type: The type of the node to erase.\n"
				"			Authorized values are: Equation, Parameter, Reaction, Species, Arithmetic,\n"
				"			Asset, LinePlot, Logic, ModifyDataStateValueEvent, Solver, Time, Trigger,\n"
				"			ValueFloat.\n"
				"Node ID: The ID of the node to erase.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to erase one node of a certain type identified
				by its ID in a target model node-based viewer context.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const EraseNodeCommandArgs& _params).
		*/
		bool Execute() override;
	};

	/*!
	@brief Command to erase a Model Node-Based Viewer Context from under a simulation.
	*/
	class EraseMNBVContextCommand : public ECellEngine::IO::Command<EraseMNBVContextCommandArgs>
	{
		Widget::ModelExplorerWidget& receiver;

	public:
		EraseMNBVContextCommand(Widget::ModelExplorerWidget& _receiver) :
			Command("eraseMNBVCtxt", 2), receiver(_receiver)
		{

		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Focus a node with a specific ID in the i-th model node-based viewer context of the model explorer.\n"
				"Usage: eraseMNBVCtxt <MNBVContext Index>\n"
				"MNBVContext Index: The index of the model node-based viewer context in the model explorer.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to erase a model node-based viewer context.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const FocusNodeCommandArgs& _params).
		*/
		bool Execute() override;
	};
	
	/*!
	@brief Command to focus a node in a Model Node-Based Viewer Context.
	*/
	class FocusNodeCommand : public ECellEngine::IO::Command<FocusNodeCommandArgs>
	{
		Widget::ModelExplorerWidget& receiver;

	public:
		FocusNodeCommand(Widget::ModelExplorerWidget& _receiver) :
			Command("focusNode", 3), receiver(_receiver)
		{
			
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Focus a node with a specific ID in the i-th model node-based viewer context of the model explorer.\n"
					"Usage: focusNode <MNBVContext Index> <Node ID> \n"
					"MNBVContext Index: The index of the model node-based viewer context in the model explorer.\n"
					"Node ID: The ID of the node to focus.\n";
		}

		/*!
		@brief Decodes the parameters, and stores them in ::args.
		@details Performs checks on @p _args to guarentee that the string command
				 is well formed.
		*/
		bool DecodeParameters(const std::vector<std::string>& _args) override;

		/*
		@brief Executes the code to add a model node-based viewer under a simulation to access this
				simulation, view it, and add actions or data.
		@details Uses the parameters defined in ::args. If you want use this
				 interface to execute the command rather than the one based on
				 the string, make sure to call ::DecodeParameters(const std::vector<std::string>& _args)
				 or to set the parameters manually with ::SetArgs(const AddMNBVContextCommandArgs& _params).
		*/
		bool Execute() override;
	};
}