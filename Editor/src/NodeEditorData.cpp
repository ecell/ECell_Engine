#include "NodeEditorData.hpp"

static ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* s_mnbvCtxt = nullptr;
#pragma region Data Global API

ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* ECellEngine::Editor::Utility::GetCurrentMNBVContext()
{
	return s_mnbvCtxt;
}

ECellEngine::Editor::Utility::NodeEditorStyle* ECellEngine::Editor::Utility::GetMNBVSyle()
{
	return &s_mnbvCtxt->style;
}

ImVec4* ECellEngine::Editor::Utility::GetNodeColors(NodeType _nodeType)
{
	return s_mnbvCtxt->style.nodeColors[_nodeType];
}

ImVec4* ECellEngine::Editor::Utility::GetPinColors(PinType _nodeType)
{
	return s_mnbvCtxt->style.pinColors[_nodeType];
}

void ECellEngine::Editor::Utility::SetCurrentMNBVContext(ModelNodeBasedViewerContext* _style)
{
	s_mnbvCtxt = _style;
}

#pragma endregion