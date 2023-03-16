#pragma once

#include <string>
#include <vector>

#include "imgui_node_editor.h"

#include "Logger.hpp"
#include "NodeEditorUtility.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	/*!
	@brief The window that contains the GUI to explore the assembled model of a
			simulation.
	@details This window displays the model as a network of connected nodes.
			 Nodes may represent model assets, solvers and the data state.
	@see It relies on a third party node editor: https://github.com/thedmd/imgui-node-editor by thedmd
	*/
	class ModelExplorerWidget : public Widget
	{
	private:
		
		/*!
		@brief The vector that contains the arguments for the 
				ECellEngine::IO::AddModuleCommand
		*/
		std::vector<std::string> addModuleCommandArray = {"addModule", "path"};

		/*!
		@brief The node editor context.
		*/
		ax::NodeEditor::EditorContext * nodeEditorCtxt = nullptr;
		std::vector<ECellEngine::Editor::Utility::LinkInfo>   links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		int nextLinkId = 100;

		void DrawModelExplorer();

	public:
		ModelExplorerWidget(Editor& _editor) :
			Widget(_editor)
		{
			ax::NodeEditor::Config nodeConfig;
			nodeEditorCtxt = ax::NodeEditor::CreateEditor(&nodeConfig);
		}

		~ModelExplorerWidget()
		{
			ax::NodeEditor::DestroyEditor(nodeEditorCtxt);
		}

		inline void Awake() override {};

		void Draw() override;
	};
}