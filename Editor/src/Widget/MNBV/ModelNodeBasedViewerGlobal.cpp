#include "Util/BinarySearch.hpp"
#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

static ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext* s_mnbvCtxt = nullptr;

void ECellEngine::Editor::Widget::MNBV::EraseDynamicLink(std::vector<Utility::MNBV::LinkData>::iterator& _dynamicLink)
{
	//Call the methods that will clear the data links via the pins.
	//We delegate to the start pin (output side) the task of caling the
	//OnDisconnect method of the end pin (input side).
	_dynamicLink->startPin->OnDisconnect(_dynamicLink->endPin);
	//_dynamicLink->endPin->OnDisconnect(_dynamicLink->startPin);

	//Erase the link data.
	s_mnbvCtxt->dynamicLinks.erase(_dynamicLink);
}

void ECellEngine::Editor::Widget::MNBV::EraseNode(const std::size_t _nodeId)
{
	//Search in the list of Arithmetic Operation Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->arithmeticOperationNodes, _nodeId)) { return; }

	//Search in the list of Asset Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->assetNodes, _nodeId)) { return; }

	//Search in the list of Equation Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->equationNodes, _nodeId)) { return; }

	//Search in the list of Line Plot Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->linePlotNodes, _nodeId)) { return; }

	//Search in the list of Line Plot Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->logicOperationNodes, _nodeId)) { return; }

	//Search in the list of Modify Value In DataState Event Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->modifyDataStateValueEventNodes, _nodeId)) { return; }

	//Search in the list of Parameter Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->parameterNodes, _nodeId)) { return; }
	
	//Search in the list of Reaction Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->reactionNodes, _nodeId)) { return; }

	//Search in the list of Solver Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->solverNodes, _nodeId)) { return; }

	//Search in the list of Species Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->speciesNodes, _nodeId)) { return; }

	//Search in the list of Time Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->timeNodes, _nodeId)) { return; }
	
	//Search in the list of Trigger Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->triggerNodes, _nodeId)) { return; }

	//Search in the list of Value Float Nodes
	if (s_mnbvCtxt->EraseNodeOfType(s_mnbvCtxt->valueFloatNodes, _nodeId)) { return; }
}

void ECellEngine::Editor::Widget::MNBV::EraseStaticLink(std::vector<Utility::MNBV::LinkData>::iterator& _staticLink)
{
	//Erase the link data.
	s_mnbvCtxt->staticLinks.erase(_staticLink);
}

ECellEngine::Editor::Utility::MNBV::NodeData* ECellEngine::Editor::Widget::MNBV::FindNodeInAll(const std::size_t _id)
{
	Utility::MNBV::NodeData* itND = nullptr;

	//Search in the list of Arithmetic Operation Nodes
	itND = FindNodeIn(_id, s_mnbvCtxt->arithmeticOperationNodes.begin(), s_mnbvCtxt->arithmeticOperationNodes.end());
	if (itND != nullptr)
	{
		return itND;
	}

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
	
	//Search in the list of Logic Operation Nodes
	itND = FindNodeIn(_id, s_mnbvCtxt->logicOperationNodes.begin(), s_mnbvCtxt->logicOperationNodes.end());
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
	itND = FindNodeIn(_id, s_mnbvCtxt->timeNodes.begin(), s_mnbvCtxt->timeNodes.end());
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

	//Search in the list of Trigger Nodes
	itND = FindNodeIn(_id, s_mnbvCtxt->triggerNodes.begin(), s_mnbvCtxt->triggerNodes.end());
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
		return &*ECellEngine::Util::BinarySearch::LowerBound(_first, _last, _id);
	}
	return nullptr;
}

ECellEngine::Editor::Utility::MNBV::NodePinData* ECellEngine::Editor::Widget::MNBV::FindNodePinInAll(const std::size_t _id)
{
	Utility::MNBV::NodePinData* itNPD = nullptr;

	//Search in the list of Arithemtic Operation Nodes
	itNPD = FindNodePinIn(_id, s_mnbvCtxt->arithmeticOperationNodes.begin(), s_mnbvCtxt->arithmeticOperationNodes.end());
	if (itNPD != nullptr)
	{
		return itNPD;
	}

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

	//Search in the list of Logic Operation Nodes
	itNPD = FindNodePinIn(_id, s_mnbvCtxt->logicOperationNodes.begin(), s_mnbvCtxt->logicOperationNodes.end());
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
	itNPD = FindNodePinIn(_id, s_mnbvCtxt->timeNodes.begin(), s_mnbvCtxt->timeNodes.end());
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

	//Search in the list of Trigger Nodes
	itNPD = FindNodePinIn(_id, s_mnbvCtxt->triggerNodes.begin(), s_mnbvCtxt->triggerNodes.end());
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
			it = ECellEngine::Util::BinarySearch::LowerBound(_first, _last, _id);

			//If the lower bound is NOT the id of the first node
			if (it != _first)
			{
				//Then, entering this section, we know that the target id is somewhere
				//within the node before the lower bound.
				it--;
				
				//We search the target id within the output pins
				itNPD = ECellEngine::Util::BinarySearch::LowerBound((*it).outputPins, (*it).outputPins + nbpins, _id);
				if (*itNPD == _id)
				{
					return itNPD;
				}
				
				//We search the target id within the input pins
				nbpins = (unsigned short)std::size(it->inputPins);
				itNPD = ECellEngine::Util::BinarySearch::LowerBound((*it).inputPins, (*it).inputPins + nbpins, _id);
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

void ECellEngine::Editor::Widget::MNBV::SetCurrentMNBVContext(ModelNodeBasedViewerContext* _ctxt)
{
	s_mnbvCtxt = _ctxt;
}

std::size_t& ECellEngine::Editor::Widget::MNBV::GetMNBVCtxtNextId()
{
	return ++s_mnbvCtxt->idProvider;
}

bool ECellEngine::Editor::Widget::MNBV::IsDynamicLinkAuthorized(Utility::MNBV::PinType _startPinType, Utility::MNBV::PinType _endPinType)
{
	return s_mnbvCtxt->authorizedDynamicLinks[_startPinType][_endPinType];
}

//void ECellEngine::Editor::Widget::MNBV::QueueEngineTASToMCmd(const std::size_t _moduleID, const std::size_t _solverID)
//{
//    s_mnbvCtxt->TASToMCmds.insert(s_mnbvCtxt->TASToMCmds.begin(),
//        ModelNodeBasedViewerContext::EngineTASToMCmdParameter(_moduleID, _solverID));
//    s_mnbvCtxt->countTASToMCmds++;
//}

void ECellEngine::Editor::Widget::MNBV::RemoveAssetNode(const std::size_t _idx)
{
	auto it = s_mnbvCtxt->assetNodes.begin() + _idx;
	s_mnbvCtxt->assetNodes.erase(it);
}

//void ECellEngine::Editor::Widget::MNBV::SendEngineTASToMCmd(const char* _simuIdx, CommandsManager* _cmdsManager)
//{
//    while (s_mnbvCtxt->countTASToMCmds)
//    {
//        _cmdsManager->InterpretCommand({
//            "tryLinkModuleWithSolver",
//            _simuIdx,
//            std::to_string(s_mnbvCtxt->TASToMCmds.back().solverID),
//            std::to_string(s_mnbvCtxt->TASToMCmds.back().moduleID) });
//        s_mnbvCtxt->TASToMCmds.pop_back();
//        s_mnbvCtxt->countTASToMCmds--;
//    }
//}