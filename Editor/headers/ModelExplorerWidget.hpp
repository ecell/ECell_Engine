#pragma once

#include <string>
#include <vector>

#include "imgui_node_editor.h"

#include "Logger.hpp"
#include "NodeEditorUtility.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class ModelExplorerWidget : public Widget
	{
	private:
		
		//bool firstUse;
		std::vector<std::string> addModuleCommandArray = {"addModule", "path"};

		ax::NodeEditor::EditorContext * nodeEditorCtxt = nullptr;
		ImVector<ECellEngine::Editor::Utility::LinkInfo>   links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		std::size_t nextLinkId = 100;

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