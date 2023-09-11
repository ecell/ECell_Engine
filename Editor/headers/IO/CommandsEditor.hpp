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
	class AddMNBVContextCommand : public ECellEngine::IO::Command<AddMNBVContextCommandArgs>
	{
		Widget::ModelExplorerWidget& receiver;

	public:
		AddMNBVContextCommand(Widget::ModelExplorerWidget& _receiver) :
			Command("addMNBVCtxt", 1), receiver(_receiver)
		{
			//fill in the args
		}

		inline const char* GetHelpMessage() const override
		{
			return "---- Add a model node-based viewer context under a simulation for the editor.\n"
				"Usage: addModule \n";
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