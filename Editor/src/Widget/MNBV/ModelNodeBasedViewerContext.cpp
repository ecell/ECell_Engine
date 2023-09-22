#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::ConserveLinkDataIntegrity()
{
	for (std::vector<Utility::MNBV::LinkData>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
	{
		it->Refresh();
	}
}

bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseAllNodesOfType(const char* _nodesTypeName) noexcept
{
	if (!strcmp(_nodesTypeName, "Equation"))
	{
		equationNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Parameter"))
	{
		parameterNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Reaction"))
	{
		reactionNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Species"))
	{
		speciesNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Arithmetic"))
	{
		arithmeticOperationNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Asset"))
	{
		assetNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "LinePlot"))
	{
		linePlotNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Logic"))
	{
		logicOperationNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "ModifyDataStateValueEvent"))
	{
		modifyDataStateValueEventNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Solver"))
	{
		solverNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Time"))
	{
		timeNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "Trigger"))
	{
		triggerNodes.clear();
		return true;
	}
	if (!strcmp(_nodesTypeName, "ValueFloat"))
	{
		valueFloatNodes.clear();
		return true;
	}
	return false;
}