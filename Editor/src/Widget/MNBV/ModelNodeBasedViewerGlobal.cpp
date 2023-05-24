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

void ECellEngine::Editor::Widget::MNBV::EraseDynamicLink(const std::size_t _linkId)
{
    //Search the Link data to erase.
    std::vector<Utility::MNBV::LinkData>::iterator link = ECellEngine::Data::BinaryOperation::LowerBound(s_mnbvCtxt->dynamicLinks.begin(), s_mnbvCtxt->dynamicLinks.end(), _linkId);
    
    //Erase the subscription of the end pin to the start pin.
    // --> data will not be transmitted from the start pin to the end pin anymore.
    link->startPin->EraseSubscriber(link->endPin);
    
    //Erase the link data.
    s_mnbvCtxt->dynamicLinks.erase(link);
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

    //Search in the list of Computed Parameter Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->computedParameterNodes.begin(), s_mnbvCtxt->computedParameterNodes.end());
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

    //Search in the list of Species Nodes
    itND = FindNodeIn(_id, s_mnbvCtxt->valueFloatNodes.begin(), s_mnbvCtxt->valueFloatNodes.end());
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

    //Search in the list of Computed Parameter Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->computedParameterNodes.begin(), s_mnbvCtxt->computedParameterNodes.end());
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

    //Search in the list of Species Nodes
    itNPD = FindNodePinIn(_id, s_mnbvCtxt->valueFloatNodes.begin(), s_mnbvCtxt->valueFloatNodes.end());
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