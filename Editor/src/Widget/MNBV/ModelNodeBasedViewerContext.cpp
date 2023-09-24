#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::ConserveLinkDataIntegrity()
{
	for (std::vector<Utility::MNBV::LinkData>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
	{
		it->Refresh();
	}
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::ArithmeticOperationNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return arithmeticOperationNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::AssetNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return assetNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::EquationNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return equationNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::LinePlotNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return linePlotNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::LogicOperationNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return logicOperationNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return modifyDataStateValueEventNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::ReactionNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return reactionNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::ParameterNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return parameterNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::SolverNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return solverNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::SpeciesNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return speciesNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::TimeNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return timeNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::TriggerNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return triggerNodes;
}

template<>
std::vector<ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData>& ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::GetNodesOfType() noexcept
{
	return valueFloatNodes;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::ArithmeticOperationNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::ArithmeticOperationNodeData>& nodes = GetNodesOfType<Utility::MNBV::ArithmeticOperationNodeData>();
	std::vector<Utility::MNBV::ArithmeticOperationNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::AssetNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::AssetNodeData>& nodes = GetNodesOfType<Utility::MNBV::AssetNodeData>();
	std::vector<Utility::MNBV::AssetNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::EquationNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::EquationNodeData>& nodes = GetNodesOfType<Utility::MNBV::EquationNodeData>();
	std::vector<Utility::MNBV::EquationNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::LinePlotNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::LinePlotNodeData>& nodes = GetNodesOfType<Utility::MNBV::LinePlotNodeData>();
	std::vector<Utility::MNBV::LinePlotNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::LogicOperationNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::LogicOperationNodeData>& nodes = GetNodesOfType<Utility::MNBV::LogicOperationNodeData>();
	std::vector<Utility::MNBV::LogicOperationNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::ModifyDataStateValueEventNodeData>& nodes = GetNodesOfType<Utility::MNBV::ModifyDataStateValueEventNodeData>();
	std::vector<Utility::MNBV::ModifyDataStateValueEventNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::ParameterNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::ParameterNodeData>& nodes = GetNodesOfType<Utility::MNBV::ParameterNodeData>();
	std::vector<Utility::MNBV::ParameterNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::ReactionNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::ReactionNodeData>& nodes = GetNodesOfType<Utility::MNBV::ReactionNodeData>();
	std::vector<Utility::MNBV::ReactionNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::SolverNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::SolverNodeData>& nodes = GetNodesOfType<Utility::MNBV::SolverNodeData>();
	std::vector<Utility::MNBV::SolverNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::SpeciesNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::SpeciesNodeData>& nodes = GetNodesOfType<Utility::MNBV::SpeciesNodeData>();
	std::vector<Utility::MNBV::SpeciesNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::TimeNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::TimeNodeData>& nodes = GetNodesOfType<Utility::MNBV::TimeNodeData>();
	std::vector<Utility::MNBV::TimeNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::TriggerNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::TriggerNodeData>& nodes = GetNodesOfType<Utility::MNBV::TriggerNodeData>();
	std::vector<Utility::MNBV::TriggerNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}

template<>
bool ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EraseNodeOfType<ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData>(const std::size_t _nodeID) noexcept
{
	std::vector<Utility::MNBV::ValueFloatNodeData>& nodes = GetNodesOfType<Utility::MNBV::ValueFloatNodeData>();
	std::vector<Utility::MNBV::ValueFloatNodeData>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(nodes.begin(), nodes.end(), _nodeID);
	if (nodeIt != nodes.end() && (std::size_t)nodeIt->id == _nodeID)
	{
		nodes.erase(nodeIt);
		return true;
	}
	return false;
}