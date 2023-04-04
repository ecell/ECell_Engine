#include "NodeEditorData.hpp"

static ECellEngine::Editor::Utility::NodeEditorStyle* s_style = nullptr;
#pragma region Data Global API

ECellEngine::Editor::Utility::NodeEditorStyle* ECellEngine::Editor::Utility::GetCurrentStyle()
{
	return s_style;
}

ImVec4* ECellEngine::Editor::Utility::GetNodeColors(NodeType _nodeType)
{
	return s_style->nodeColors[_nodeType];
}

ImVec4* ECellEngine::Editor::Utility::GetPinColors(PinType _nodeType)
{
	return s_style->pinColors[_nodeType];
}

void ECellEngine::Editor::Utility::SetCurrentStyle(NodeEditorStyle* _style)
{
	s_style = _style;
}

#pragma endregion