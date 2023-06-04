#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

static ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext* s_mnbvCtxt = nullptr;

void ECellEngine::Editor::Widget::MNBV::AddAssetNode(ECellEngine::Data::Module* _module)
{
    s_mnbvCtxt->assetNodes.push_back(Utility::MNBV::AssetNodeData(_module));
}

void ECellEngine::Editor::Widget::MNBV::AddSolverNode(ECellEngine::Solvers::Solver* _solver)
{
    s_mnbvCtxt->solverNodes.push_back(Utility::MNBV::SolverNodeData(_solver));
}

void ECellEngine::Editor::Widget::MNBV::CurrentMNBVContextDraw(ECellEngine::Core::Simulation* _simulation)
{
    s_mnbvCtxt->Draw(_simulation);
}

void ECellEngine::Editor::Widget::MNBV::EraseDynamicLink(std::vector<Utility::MNBV::LinkData>::iterator& _dynamicLink)
{
    //Call the methods that will clear the data links via the pins.
    _dynamicLink->startPin->OnDisconnect(_dynamicLink->endPin);
    _dynamicLink->endPin->OnDisconnect(_dynamicLink->startPin);

    //Erase the link data.
    s_mnbvCtxt->dynamicLinks.erase(_dynamicLink);
}

void ECellEngine::Editor::Widget::MNBV::EraseNode(const std::size_t _nodeId)
{
    //Search in the list of Asset Nodes
    std::vector<Utility::MNBV::AssetNodeData>::iterator itAND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->assetNodes.begin(), s_mnbvCtxt->assetNodes.end(), _nodeId);
    if (itAND != s_mnbvCtxt->assetNodes.end() && (std::size_t)itAND->id == _nodeId)
    {
        s_mnbvCtxt->assetNodes.erase(itAND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: AssetNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Equation Nodes
    std::vector<Utility::MNBV::EquationNodeData>::iterator itCPND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->equationNodes.begin(), s_mnbvCtxt->equationNodes.end(), _nodeId);
    if (itCPND != s_mnbvCtxt->equationNodes.end() && (std::size_t)itCPND->id == _nodeId)
    {
        s_mnbvCtxt->equationNodes.erase(itCPND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: EquationNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Line Plot Nodes
    std::vector<Utility::MNBV::LinePlotNodeData>::iterator itLPND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->linePlotNodes.begin(), s_mnbvCtxt->linePlotNodes.end(), _nodeId);
    if (itLPND != s_mnbvCtxt->linePlotNodes.end() && (std::size_t)itLPND->id == _nodeId)
    {
        s_mnbvCtxt->linePlotNodes.erase(itLPND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: LinePlotNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Modify Value In DataState Event Nodes
    std::vector<Utility::MNBV::ModifyDataStateValueEventNodeData>::iterator itMDSTVEND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->modifyDataStateValueEventNodes.begin(), s_mnbvCtxt->modifyDataStateValueEventNodes.end(), _nodeId);
    if (itMDSTVEND != s_mnbvCtxt->modifyDataStateValueEventNodes.end() && (std::size_t)itMDSTVEND->id == _nodeId)
    {
        s_mnbvCtxt->modifyDataStateValueEventNodes.erase(itMDSTVEND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: ModifyDataStateValueEventNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Reaction Nodes
    std::vector<Utility::MNBV::ReactionNodeData>::iterator itRND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->reactionNodes.begin(), s_mnbvCtxt->reactionNodes.end(), _nodeId);
    if (itRND != s_mnbvCtxt->reactionNodes.end() && (std::size_t)itRND->id == _nodeId)
    {
        s_mnbvCtxt->reactionNodes.erase(itRND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: ReactionNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Parameter Nodes
    std::vector<Utility::MNBV::ParameterNodeData>::iterator itSPaND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->parameterNodes.begin(), s_mnbvCtxt->parameterNodes.end(), _nodeId);
    if (itSPaND != s_mnbvCtxt->parameterNodes.end() && (std::size_t)itSPaND->id == _nodeId)
    {
        s_mnbvCtxt->parameterNodes.erase(itSPaND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: ParameterNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Simulation Time Nodes
    std::vector<Utility::MNBV::SimulationTimeNodeData>::iterator itSTND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->simulationTimeNodes.begin(), s_mnbvCtxt->simulationTimeNodes.end(), _nodeId);
    if (itSTND != s_mnbvCtxt->simulationTimeNodes.end() && (std::size_t)itSTND->id == _nodeId)
    {
        s_mnbvCtxt->simulationTimeNodes.erase(itSTND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: SimulationTimeNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Solver Nodes
    std::vector<Utility::MNBV::SolverNodeData>::iterator itSoND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->solverNodes.begin(), s_mnbvCtxt->solverNodes.end(), _nodeId);
    if (itSoND != s_mnbvCtxt->solverNodes.end() && (std::size_t)itSoND->id == _nodeId)
    {
        s_mnbvCtxt->solverNodes.erase(itSoND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: SolverNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Species Nodes
    std::vector<Utility::MNBV::SpeciesNodeData>::iterator itSpND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->speciesNodes.begin(), s_mnbvCtxt->speciesNodes.end(), _nodeId);
    if (itSpND != s_mnbvCtxt->speciesNodes.end() && (std::size_t)itSpND->id == _nodeId)
    {
        s_mnbvCtxt->speciesNodes.erase(itSpND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: SpeciesNodeData " + std::to_string(_nodeId));
        return;
    }

    //Search in the list of Value Float Nodes
    std::vector<Utility::MNBV::ValueFloatNodeData>::iterator itVFND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->valueFloatNodes.begin(), s_mnbvCtxt->valueFloatNodes.end(), _nodeId);
    if (itVFND != s_mnbvCtxt->valueFloatNodes.end() && (std::size_t)itVFND->id == _nodeId)
    {
        s_mnbvCtxt->valueFloatNodes.erase(itVFND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: ValueFloatNodeData " + std::to_string(_nodeId));
        return;
    }
    
    //Search in the list of Watcher Nodes
    std::vector<Utility::MNBV::WatcherNodeData>::iterator itWND = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->watcherNodes.begin(), s_mnbvCtxt->watcherNodes.end(), _nodeId);
    if (itWND != s_mnbvCtxt->watcherNodes.end() && (std::size_t)itWND->id == _nodeId)
    {
        s_mnbvCtxt->watcherNodes.erase(itWND);
        //ECellEngine::Logging::Logger::GetSingleton().LogDebug("EraseNode: WatcherNodeData " + std::to_string(_nodeId));
        return;
    }
}

void ECellEngine::Editor::Widget::MNBV::EraseStaticLink(std::vector<Utility::MNBV::LinkData>::iterator& _staticLink)
{
    //Erase the link data.
    s_mnbvCtxt->staticLinks.erase(_staticLink);
}

ECellEngine::Editor::Utility::MNBV::NodeData* ECellEngine::Editor::Widget::MNBV::FindNodeInAll(const std::size_t _id)
{
    Utility::MNBV::NodeData* itND = nullptr;

    //Search in the list of Asset Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->assetNodes.begin(), s_mnbvCtxt->assetNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Equation Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->equationNodes.begin(), s_mnbvCtxt->equationNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Line Plot Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->linePlotNodes.begin(), s_mnbvCtxt->linePlotNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Modify DataState Value Event Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->modifyDataStateValueEventNodes.begin(), s_mnbvCtxt->modifyDataStateValueEventNodes.end());
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

    //Search in the list of Parameter Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->parameterNodes.begin(), s_mnbvCtxt->parameterNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

     //Search in the list of Simulation Time Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->simulationTimeNodes.begin(), s_mnbvCtxt->simulationTimeNodes.end());
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

    //Search in the list of Value Float Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->valueFloatNodes.begin(), s_mnbvCtxt->valueFloatNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    //Search in the list of Watcher Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->watcherNodes.begin(), s_mnbvCtxt->watcherNodes.end());
    if (itND != nullptr)
    {
        return itND;
    }

    return nullptr;
}

template<class ForwardIt>
ECellEngine::Editor::Utility::MNBV::NodeData* ECellEngine::Editor::Widget::MNBV::FindNodeIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
{
    if (_first != _last)
    {   
        return &*ECellEngine::Data::BinaryOperation::LowerBound(_first, _last, _id);
    }
    return nullptr;
}

ECellEngine::Editor::Utility::MNBV::NodePinData* ECellEngine::Editor::Widget::MNBV::FindNodePinInAll(const std::size_t _id)
{
    Utility::MNBV::NodePinData* itNPD = nullptr;

    //Search in the list of Asset Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->assetNodes.begin(), s_mnbvCtxt->assetNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Equation Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->equationNodes.begin(), s_mnbvCtxt->equationNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Line Plot Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->linePlotNodes.begin(), s_mnbvCtxt->linePlotNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Modify DataState Value Event Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->modifyDataStateValueEventNodes.begin(), s_mnbvCtxt->modifyDataStateValueEventNodes.end());
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

    //Search in the list of Parameter Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->parameterNodes.begin(), s_mnbvCtxt->parameterNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Simulation Time Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->simulationTimeNodes.begin(), s_mnbvCtxt->simulationTimeNodes.end());
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

    //Search in the list of Value FLoat Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->valueFloatNodes.begin(), s_mnbvCtxt->valueFloatNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    //Search in the list of Watcher Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->watcherNodes.begin(), s_mnbvCtxt->watcherNodes.end());
    if (itNPD != nullptr)
    {
        return itNPD;
    }

    return nullptr;
}

template<class ForwardIt>
ECellEngine::Editor::Utility::MNBV::NodePinData* ECellEngine::Editor::Widget::MNBV::FindNodePinIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
{
    Utility::MNBV::NodePinData* itNPD;

    //if length of the space to search is greater than 0
    //The list/array has at least 1 element
    if (_first != _last)
    {
        ForwardIt it = _last;
        it--;
        unsigned short nbpins = (unsigned short)std::size(it->outputPins);

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
                nbpins = (unsigned short)std::size(it->inputPins);
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

ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext* ECellEngine::Editor::Widget::MNBV::GetCurrentMNBVContext()
{
    return s_mnbvCtxt;
}

std::vector<ECellEngine::Editor::Utility::MNBV::LinkData>& ECellEngine::Editor::Widget::MNBV::GetDynamicLinks()
{
    return s_mnbvCtxt->dynamicLinks;
}

std::vector<ECellEngine::Editor::Utility::MNBV::LinkData>& ECellEngine::Editor::Widget::MNBV::GetStaticLinks()
{
    return s_mnbvCtxt->staticLinks;
}

ECellEngine::Editor::Utility::MNBV::NodeEditorStyle* ECellEngine::Editor::Widget::MNBV::GetMNBVStyle()
{
    return &s_mnbvCtxt->style;
}

ImVec4* ECellEngine::Editor::Widget::MNBV::GetNodeColors(Utility::MNBV::NodeType _nodeType)
{
    return s_mnbvCtxt->style.nodeColors[_nodeType];
}

ImVec4* ECellEngine::Editor::Widget::MNBV::GetPinColors(Utility::MNBV::PinType _pinType)
{
    return s_mnbvCtxt->style.pinColors[_pinType];
}

void ECellEngine::Editor::Widget::MNBV::SetCurrentMNBVContext(ModelNodeBasedViewerContext* _ctxt)
{
    s_mnbvCtxt = _ctxt;
}

std::size_t& ECellEngine::Editor::Widget::MNBV::GetMNBVCtxtNextId()
{
    return ++(s_mnbvCtxt->uniqueId);
}

bool ECellEngine::Editor::Widget::MNBV::IsDynamicLinkAuthorized(Utility::MNBV::PinType _startPinType, Utility::MNBV::PinType _endPinType)
{
    return s_mnbvCtxt->authorizedDynamicLinks[_startPinType][_endPinType];
}

void ECellEngine::Editor::Widget::MNBV::QueueEngineTASToMCmd(const char* _moduleName, const char* _solverName)
{
    s_mnbvCtxt->TASToMCmds.insert(s_mnbvCtxt->TASToMCmds.begin(),
        ModelNodeBasedViewerContext::EngineTASToMCmdParameter(_moduleName, _solverName));
    s_mnbvCtxt->countTASToMCmds++;
}

void ECellEngine::Editor::Widget::MNBV::RemoveAssetNode(const std::size_t _idx)
{
    auto it = s_mnbvCtxt->assetNodes.begin() + _idx;
    s_mnbvCtxt->assetNodes.erase(it);
}

void ECellEngine::Editor::Widget::MNBV::SendEngineTASToMCmd(const char* _simuIdx, CommandsManager* _cmdsManager)
{
    while (s_mnbvCtxt->countTASToMCmds > 0)
    {
        _cmdsManager->interpretCommand({ "tryAttachSolver", _simuIdx, s_mnbvCtxt->TASToMCmds.back().solverName, s_mnbvCtxt->TASToMCmds.back().moduleName });
        s_mnbvCtxt->countTASToMCmds--;
    }
}