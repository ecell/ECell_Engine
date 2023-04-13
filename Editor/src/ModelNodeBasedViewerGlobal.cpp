#include "ModelNodeBasedViewerContext.hpp"

static ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* s_mnbvCtxt = nullptr;

void ECellEngine::Editor::Utility::CurrentMNBVContextDraw(ECellEngine::Data::DataState* _dataState)
{
    s_mnbvCtxt->Draw(_dataState);
}

ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* ECellEngine::Editor::Utility::GetCurrentMNBVContext()
{
    return s_mnbvCtxt;
}

ECellEngine::Editor::Utility::NodeEditorStyle* ECellEngine::Editor::Utility::GetMNBVStyle()
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

void ECellEngine::Editor::Utility::SetCurrentMNBVContext(ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* _ctxt)
{
    s_mnbvCtxt = _ctxt;
}

std::size_t& ECellEngine::Editor::Utility::GetMNBVCtxtNextId()
{
    ECellEngine::Logging::Logger::GetSingleton().LogDebug("GetMNBVCtxtNextId: " + std::to_string(s_mnbvCtxt->uniqueId));
    return ++(s_mnbvCtxt->uniqueId);
}

void ECellEngine::Editor::Utility::AddAssetNode(ECellEngine::Data::Module* _module)
{
    s_mnbvCtxt->assetNodes.push_back(ECellEngine::Editor::Utility::AssetNodeData(_module));
}

void ECellEngine::Editor::Utility::AddSolverNode(ECellEngine::Solvers::Solver* _solver)
{
    s_mnbvCtxt->solverNodes.push_back(ECellEngine::Editor::Utility::SolverNodeData(_solver));
}

void ECellEngine::Editor::Utility::RemoveAssetNode(const std::size_t _idx)
{
    auto it = s_mnbvCtxt->assetNodes.begin() + _idx;
    s_mnbvCtxt->assetNodes.erase(it);
}