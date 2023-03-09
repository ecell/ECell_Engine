#pragma once

#include <string>
#include <vector>

#include "imgui_node_editor.h"

#include "Logger.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	// Struct to hold basic information about connection between
	// pins. Note that connection (aka. link) has its own ID.
	// This is useful later with dealing with selections, deletion
	// or other operations.
	struct LinkInfo
	{
		ax::NodeEditor::LinkId Id;
		ax::NodeEditor::PinId  InputId;
		ax::NodeEditor::PinId  OutputId;
	};

	class ModelExplorerWidget : public Widget
	{
	private:
		
		//bool firstUse;
		std::vector<std::string> addModuleCommandArray = {"addModule", "path"};

		ax::NodeEditor::EditorContext * m_Context = nullptr;
		ImVector<LinkInfo>   m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		int                  m_NextLinkId = 100;

		inline void ImGuiEx_BeginColumn()
		{
			ImGui::BeginGroup();
		}

		inline void ImGuiEx_NextColumn()
		{
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
		}

		inline void ImGuiEx_EndColumn()
		{
			ImGui::EndGroup();
		}

		void DrawModelExplorer();

	public:
		ModelExplorerWidget(Editor& _editor) :
			Widget(_editor)
		{
			ax::NodeEditor::Config nodeConfig;
			m_Context = ax::NodeEditor::CreateEditor(&nodeConfig);
		}

		~ModelExplorerWidget()
		{
			ax::NodeEditor::DestroyEditor(m_Context);
		}

		inline void Awake() override {};

		void Draw() override;
	};
}