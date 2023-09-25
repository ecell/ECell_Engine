#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::ConserveLinkDataIntegrity()
{
	for (std::vector<Utility::MNBV::LinkData>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
	{
		it->Refresh();
	}
}

bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseAllNodesOfType(const char* _nodeType) noexcept
{
	if (!strcmp(_nodeType, "Equation"))
	{
		equationNodes.clear();
	}
	else if (!strcmp(_nodeType, "Parameter"))
	{
		parameterNodes.clear();
	}
	else if (!strcmp(_nodeType, "Reaction"))
	{
		reactionNodes.clear();
	}
	else if (!strcmp(_nodeType, "Species"))
	{
		speciesNodes.clear();
	}
	else if (!strcmp(_nodeType, "Arithmetic"))
	{
		arithmeticOperationNodes.clear();
	}
	else if (!strcmp(_nodeType, "Asset"))
	{
		assetNodes.clear();
	}
	else if (!strcmp(_nodeType, "LinePlot"))
	{
		linePlotNodes.clear();
	}
	else if (!strcmp(_nodeType, "Logic"))
	{
		logicOperationNodes.clear();
	}
	else if (!strcmp(_nodeType, "ModifyDataStateValueEvent"))
	{
		modifyDataStateValueEventNodes.clear();
	}
	else if (!strcmp(_nodeType, "Solver"))
	{
		solverNodes.clear();
	}
	else if (!strcmp(_nodeType, "Time"))
	{
		timeNodes.clear();
	}
	else if (!strcmp(_nodeType, "Trigger"))
	{
		triggerNodes.clear();
	}
	else if (!strcmp(_nodeType, "ValueFloat"))
	{
		valueFloatNodes.clear();
	}
	else
	{
		return false;
	}
	return true;
}

bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType(const char* _nodeType, const std::size_t _nodeID) noexcept
{
	if (!strcmp(_nodeType, "Equation"))
	{
		if (!EraseNodeOfType(equationNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Equation\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Parameter"))
	{
		if (!EraseNodeOfType(parameterNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Parameter\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Reaction"))
	{
		if (!EraseNodeOfType(reactionNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Reaction\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Species"))
	{
		if (!EraseNodeOfType(speciesNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Species\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Arithmetic"))
	{
		if (!EraseNodeOfType(arithmeticOperationNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Arithmetic\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Asset"))
	{
		if (!EraseNodeOfType(assetNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Asset\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "LinePlot"))
	{
		if (!EraseNodeOfType(linePlotNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"LinePlot\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Logic"))
	{
		if (!EraseNodeOfType(logicOperationNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Logic\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "ModifyDataStateValueEvent"))
	{
		if (!EraseNodeOfType(modifyDataStateValueEventNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"ModifyDataStateValueEvent\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Solver"))
	{
		if (!EraseNodeOfType(solverNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Solver\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Time"))
	{
		if (!EraseNodeOfType(timeNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Time\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "Trigger"))
	{
		if (!EraseNodeOfType(triggerNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"Trigger\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else if (!strcmp(_nodeType, "ValueFloat"))
	{
		if (!EraseNodeOfType(valueFloatNodes, _nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Could not erase node of type \"ValueFloat\" with ID %llu.", _nodeID);
			return false;
		}
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("EraseNodeOfType: Invalid node type \"%s\".", _nodeType);
		return false;
	}

	return true;
}