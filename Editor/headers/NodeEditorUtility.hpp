#pragma once
#include "imgui_node_editor.h"

namespace ECellEngine::Editor::Utility
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

	struct NodeEditorUtility
	{
		 
		inline static void BeginColumn()
		{
			ImGui::BeginGroup();
		}

		inline static void NextColumn()
		{
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
		}

		inline static void EndColumn()
		{
			ImGui::EndGroup();
		}
	};
}