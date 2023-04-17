#pragma once

#include "BinaryOperatedVector.hpp"
#include "SBMLModule.hpp" //eventually includes DataState.hpp
#include "NodeEditorStyle.hpp"

namespace ECellEngine::Editor::Utility
{
	//forward declaration
	struct ModelNodeBasedViewerContext;
	struct NodePinData;
	struct NodeData;

	/*!
	@brief Adds an ECellEngine::Editor::Utility::AssetNodeInfo in
			ModelNodeBasedViewerContext::assetNodes.
	@details It has for consequence to draw an asset node in the editor.
	*/
	void AddAssetNode(ECellEngine::Data::Module* _simulationSource);

	void CurrentMNBVContextDraw(ECellEngine::Data::DataState* _dataState);

	ModelNodeBasedViewerContext* GetCurrentMNBVContext();

	/*!
	@brief Adds an ECellEngine::Editor::Utility::SolverNodeInfo in
			ModelNodeBasedViewerContext::solverNodes.
	@details It has for consequence to draw a solver node in the editor.
	*/
	void AddSolverNode(ECellEngine::Solvers::Solver* _simulationSource);

	NodePinData* FindNodePinInAll(const std::size_t _id);

	template<class ForwardIt>
	NodePinData* FindNodePinIn(const std::size_t _id, ForwardIt _first, ForwardIt _last);

	NodeEditorStyle* GetMNBVStyle();

	std::size_t& GetMNBVCtxtNextId();

	ImVec4* GetNodeColors(NodeType _nodeType);

	inline char* GetNodeColorTypeName(NodeColorType _nodeColorType)
	{
		switch (_nodeColorType)
		{
		case NodeColorType_Bg:
			return "NodeColorType_Bg";
		case NodeColorType_Border:
			return "NodeColorType_Border";
			/*case NodeColorType_BorderHovered:
				return "NodeColorType_BorderHovered";*/
				/*case NodeColorType_BorderSelected:
					return "NodeColorType_BorderSelected";*/
		case NodeColorType_HeaderBg:
			return "NodeColorType_HeaderBg";
		case NodeColorType_HeaderActivated:
			return "NodeColorType_HeaderActivated";
		case NodeColorType_HeaderHovered:
			return "NodeColorType_HeaderHovered";
		}
	}

	inline char* GetNodeTypeName(NodeType _nodeType)
	{
		switch (_nodeType)
		{
		case NodeType_Default:
			return "NodeType_Default";
		case NodeType_Asset:
			return "NodeType_Asset";
		case NodeType_Parameter:
			return "NodeType_Parameter";
		case NodeType_Reaction:
			return "NodeType_Reaction";
		case NodeType_Solver:
			return "NodeType_Solver";
		case NodeType_Species:
			return "NodeType_Species";
		}
	}

	ImVec4* GetPinColors(PinType _nodeType);

	inline char* GetPinColorTypeName(PinColorType _pinColorType)
	{
		switch (_pinColorType)
		{
		case PinColorType_BgActivated:
			return "PinColorType_BgActivated";
		case PinColorType_BgInactivated:
			return "PinColorType_BgInactivated";
		case PinColorType_Border:
			return "PinColorType_Border";
		}
	}

	inline char* GetPinTypeName(PinType _pinType)
	{
		switch (_pinType)
		{
		case PinType_Default:
			return "PinType_Default";
		case PinType_Asset:
			return "PinType_Asset";
		case PinType_Parameter:
			return "PinType_Parameter";
		case PinType_Reaction:
			return "PinType_Reaction";
		case PinType_Solver:
			return "PinType_Solver";
		case PinType_Species:
			return "PinType_Species";
		}
	}

	bool IsDynamicLinkAuthorized(PinType _startPinType, PinType _endPinType);

	/*!
	@brief Removes as asset node from ModelNodeBasedViewerContext::assetNodes
	@param _idx The index of the node to erase from ModelNodeBasedViewerContext::assetNodes.
	*/
	void RemoveAssetNode(const std::size_t _idx);

	void SetCurrentMNBVContext(ModelNodeBasedViewerContext* _ctxt);
}

