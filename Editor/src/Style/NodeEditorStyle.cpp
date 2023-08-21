#include "imgui_node_editor.h"
#include "Style/NodeEditorStyle.hpp"

void ECellEngine::Editor::Style::NodeEditorStyle::PopNodeStyle()
{
	ImGui::PopStyleColor(4);
	ax::NodeEditor::PopStyleColor(2);
}

void ECellEngine::Editor::Style::NodeEditorStyle::PushNodeStyle(const ImVec4 _colorSet[])
{
	ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, _colorSet[Style::NodeColorType_Bg]);
	ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, _colorSet[Style::NodeColorType_Border]);
	ImGui::PushStyleColor(ImGuiCol_Button, _colorSet[Style::NodeColorType_Bg]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, _colorSet[Style::NodeColorType_HeaderActivated]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _colorSet[Style::NodeColorType_HeaderHovered]);
	ImGui::PushStyleColor(ImGuiCol_Separator, _colorSet[Style::NodeColorType_Border]);
}