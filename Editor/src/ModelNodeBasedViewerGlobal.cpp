#include "ModelNodeBasedViewerContext.hpp"

static ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* s_mnbvCtxt = nullptr;

void ECellEngine::Editor::Utility::AddAssetNode(ECellEngine::Data::Module* _module)
{
    s_mnbvCtxt->assetNodes.push_back(ECellEngine::Editor::Utility::AssetNodeData(_module));
}

void ECellEngine::Editor::Utility::AddSolverNode(ECellEngine::Solvers::Solver* _solver)
{
    s_mnbvCtxt->solverNodes.push_back(ECellEngine::Editor::Utility::SolverNodeData(_solver));
}

void ECellEngine::Editor::Utility::CurrentMNBVContextDraw(ECellEngine::Data::DataState* _dataState)
{
    s_mnbvCtxt->Draw(_dataState);
}

ECellEngine::Editor::Utility::NodeData* ECellEngine::Editor::Utility::FindNodeInAll(const std::size_t _id)
{
    NodeData* itND = nullptr;

    //Search in the list of Asset Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->assetNodes.begin(), s_mnbvCtxt->assetNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Computed Parameter Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->computedParameterNodes.begin(), s_mnbvCtxt->computedParameterNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Reaction Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->reactionNodes.begin(), s_mnbvCtxt->reactionNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Simple Parameter Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->simpleParameterNodes.begin(), s_mnbvCtxt->simpleParameterNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Solver Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->solverNodes.begin(), s_mnbvCtxt->solverNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Species Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->speciesNodes.begin(), s_mnbvCtxt->speciesNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    return nullptr;
}

template<class ForwardIt>
ECellEngine::Editor::Utility::NodeData* ECellEngine::Editor::Utility::FindNodeIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
{
    if (_first != _last)
    {   
        return &*ECellEngine::Data::BinaryOperation::LowerBound(_first, _last, _id);
 
    }
    return nullptr;
}

ECellEngine::Editor::Utility::NodePinData* ECellEngine::Editor::Utility::FindNodePinInAll(const std::size_t _id)
{
    NodePinData* itNPD = nullptr;

    //Search in the list of Asset Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->assetNodes.begin(), s_mnbvCtxt->assetNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Computed Parameter Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->computedParameterNodes.begin(), s_mnbvCtxt->computedParameterNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Reaction Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->reactionNodes.begin(), s_mnbvCtxt->reactionNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Simple Parameter Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->simpleParameterNodes.begin(), s_mnbvCtxt->simpleParameterNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Solver Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->solverNodes.begin(), s_mnbvCtxt->solverNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Species Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->speciesNodes.begin(), s_mnbvCtxt->speciesNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    return nullptr;
}

template<class ForwardIt>
ECellEngine::Editor::Utility::NodePinData* ECellEngine::Editor::Utility::FindNodePinIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
{
    NodePinData* itNPD;

    //if length of the space to search is greater than 0
    //The list/array has at least 1 element
    if (_first != _last)
    {
        ForwardIt it = _last;
        it--;
        unsigned short nbpins = std::size(it->outputPins);

        //If the target id is lower than the highest id of interest
        //INFO: by convention when creating a NodeData, we finish by an output pin and not an input pin.
        if (_id <= it->outputPins[nbpins - 1])
        {
            //Entering this section there is a possibility that the target id is indeed within the list/array
            //We do the search among the nodes data
            it = ECellEngine::Data::BinaryOperation::LowerBound(_first, _last, _id);

            //If the lower bound is NOT the id of the first node
            if (it != _first)
            {
                //Then, entering this section, we know that the target id is somewhere
                //within the node before the lower bound.
                it--;
                
                //We search the target id within the output pins
                itNPD = ECellEngine::Data::BinaryOperation::LowerBound((*it).outputPins, (*it).outputPins + nbpins, _id);
                if (*itNPD == _id)
                {
                    return itNPD;
                }
                
                //We search the target id within the input pins
                nbpins = std::size(it->inputPins);
                itNPD = ECellEngine::Data::BinaryOperation::LowerBound((*it).inputPins, (*it).inputPins + nbpins, _id);
                if (*itNPD == _id)
                {
                    return itNPD;
                }
            }
        }
    }
    return nullptr;
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

bool ECellEngine::Editor::Utility::IsDynamicLinkAuthorized(PinType _startPinType, PinType _endPinType)
{
    return s_mnbvCtxt->authorizedDynamicLinks[_startPinType][_endPinType];
}

void ECellEngine::Editor::Utility::QueueEngineTASToMCmd(const char* _moduleName, const char* _solverName)
{
    s_mnbvCtxt->TASToMCmds.insert(s_mnbvCtxt->TASToMCmds.begin(),
        ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::EngineTASToMCmdParameter(_moduleName, _solverName));
    s_mnbvCtxt->countTASToMCmds++;
}

void ECellEngine::Editor::Utility::RemoveAssetNode(const std::size_t _idx)
{
    auto it = s_mnbvCtxt->assetNodes.begin() + _idx;
    s_mnbvCtxt->assetNodes.erase(it);
}

void ECellEngine::Editor::Utility::SendEngineTASToMCmd(const char* _simuIdx, CommandsManager* _cmdsManager)
{
    while (s_mnbvCtxt->countTASToMCmds > 0)
    {
        _cmdsManager->interpretCommand({ "tryAttachSolver", _simuIdx, s_mnbvCtxt->TASToMCmds.back().solverName, s_mnbvCtxt->TASToMCmds.back().moduleName });
        s_mnbvCtxt->countTASToMCmds--;
    }
}