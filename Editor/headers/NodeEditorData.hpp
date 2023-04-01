#pragma once
#include "imgui_node_editor.h"

#include "SBMLModule.hpp"


namespace ECellEngine::Editor::Utility
{
#pragma region Custom Color Style
	
	enum LinkStyleColor
	{
		LinkStyleColor_BorderHovered,
		LinkStyleColor_BorderSelected,

		LinkStyleColor_Count
	};
	
	enum NodeStyleColor
	{
		NodeStyleColor_Bg,
		NodeStyleColor_Border,
		//NodeStyleColor_BorderHovered,
		//NodeStyleColor_BorderSelected,
		NodeStyleColor_HeaderBg,
		NodeStyleColor_HeaderActivated,
		NodeStyleColor_HeaderHovered,

		NodeStyleColor_Count
	};

	enum PinStyleColor
	{
		PinStyleColor_Rect,
		PinStyleColor_RectBorder,

		PinStyleColor_Count
	};

	struct NodeEditorStyleColors
	{
		ImVec4 defaultNode[NodeStyleColor_Count];
		ImVec4 assetNode[NodeStyleColor_Count];
		ImVec4 parameterNode[NodeStyleColor_Count];
		ImVec4 reactionNode[NodeStyleColor_Count];
		ImVec4 speciesNode[NodeStyleColor_Count];

		/*ImVec4 assetLink[LinkStyleColor_Count];
		ImVec4 parameterLink[LinkStyleColor_Count];
		ImVec4 reactionLink[LinkStyleColor_Count];
		ImVec4 speciesLink[LinkStyleColor_Count];

		ImVec4 assetPin[PinStyleColor_Count];
		ImVec4 parameterPin[PinStyleColor_Count];
		ImVec4 reactionPin[PinStyleColor_Count];
		ImVec4 speciesPin[PinStyleColor_Count];*/

		NodeEditorStyleColors(ax::NodeEditor::Style& _defaultStyle)
		{
			defaultNode[NodeStyleColor_Bg] =				_defaultStyle.Colors[ax::NodeEditor::StyleColor_NodeBg];
			defaultNode[NodeStyleColor_Border] =			_defaultStyle.Colors[ax::NodeEditor::StyleColor_NodeBorder];
			//defaultNode[NodeStyleColor_BorderHovered] =		_defaultStyle.Colors[ax::NodeEditor::StyleColor_HovNodeBorder];
			//defaultNode[NodeStyleColor_BorderSelected] =	_defaultStyle.Colors[ax::NodeEditor::StyleColor_SelNodeBorder];
			defaultNode[NodeStyleColor_HeaderBg] =			_defaultStyle.Colors[ax::NodeEditor::StyleColor_NodeBorder];
			defaultNode[NodeStyleColor_HeaderActivated] =	_defaultStyle.Colors[ax::NodeEditor::StyleColor_SelNodeBorder];
			defaultNode[NodeStyleColor_HeaderHovered] =		_defaultStyle.Colors[ax::NodeEditor::StyleColor_HovNodeBorder];
			
			assetNode[NodeStyleColor_Bg] =					ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
			assetNode[NodeStyleColor_Border] =				ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			//assetNode[NodeStyleColor_BorderHovered] =		ImVec4(0.f, 1.f, 0.f, 1.f);
			//assetNode[NodeStyleColor_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			assetNode[NodeStyleColor_HeaderBg] =			ImVec4(0.7f, 0.7f, 0.7f, 0.25f);
			assetNode[NodeStyleColor_HeaderActivated] =		ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
			assetNode[NodeStyleColor_HeaderHovered] =		ImVec4(0.7f, 0.7f, 0.7f, 0.5f);

			parameterNode[NodeStyleColor_Bg] =				_defaultStyle.Colors[ax::NodeEditor::StyleColor_NodeBg];
			parameterNode[NodeStyleColor_Border] =			ImVec4(1.f, 0.f, 0.f, 1.f);
			//parameterNode[NodeStyleColor_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//parameterNode[NodeStyleColor_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			parameterNode[NodeStyleColor_HeaderBg] =		ImVec4(1.f, 0.f, 0.f, 1.f);;
			parameterNode[NodeStyleColor_HeaderActivated] = ImVec4(1.f, 0.f, 0.f, 1.f);;
			parameterNode[NodeStyleColor_HeaderHovered] =	ImVec4(1.f, 0.f, 0.f, 1.f);;

			reactionNode[NodeStyleColor_Bg] =				_defaultStyle.Colors[ax::NodeEditor::StyleColor_NodeBg];
			reactionNode[NodeStyleColor_Border] =			ImVec4(1.f, 0.f, 0.f, 1.f);
			//reactionNode[NodeStyleColor_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//reactionNode[NodeStyleColor_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			reactionNode[NodeStyleColor_HeaderBg] =			ImVec4(1.f, 0.f, 0.f, 1.f);
			reactionNode[NodeStyleColor_HeaderActivated] =	ImVec4(1.f, 0.f, 0.f, 1.f);
			reactionNode[NodeStyleColor_HeaderHovered] =	ImVec4(1.f, 0.f, 0.f, 1.f);

			speciesNode[NodeStyleColor_Bg] =				ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
			speciesNode[NodeStyleColor_Border] =			ImVec4(1.f, 0.f, 0.f, 1.f);
			//speciesNode[NodeStyleColor_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);.f);
			//speciesNode[NodeStyleColor_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			speciesNode[NodeStyleColor_HeaderBg] =			ImVec4(1.f, 0.f, 0.f, 0.25f);
			speciesNode[NodeStyleColor_HeaderActivated] =	ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
			speciesNode[NodeStyleColor_HeaderHovered] =		ImVec4(1.f, 0.f, 0.f, 0.5f);

		}

		inline char* GetLinkStyleColorName(LinkStyleColor _linkStyleColorType)
		{
			switch (_linkStyleColorType)
			{
			case LinkStyleColor_BorderHovered:
				return "LinkStyleColor_BorderHovered";
			case LinkStyleColor_BorderSelected:
				return "LinkStyleColor_BorderSelected";
			}
		}

		inline char* GetNodeStyleColorName(NodeStyleColor _nodeStyleColorType)
		{
			switch (_nodeStyleColorType)
			{
			case NodeStyleColor_Bg:
				return "NodeStyleColor_Bg";
			case NodeStyleColor_Border:
				return "NodeStyleColor_Border";
			/*case NodeStyleColor_BorderHovered:
				return "NodeStyleColor_BorderHovered";*/
			/*case NodeStyleColor_BorderSelected:
				return "NodeStyleColor_BorderSelected";*/
			case NodeStyleColor_HeaderBg:
				return "NodeStyleColor_HeaderBg";
			case NodeStyleColor_HeaderActivated:
				return "NodeStyleColor_HeaderActivated";
			case NodeStyleColor_HeaderHovered:
				return "NodeStyleColor_HeaderHovered";
			}
		}
		
		inline char* GetPinStyleColorName(PinStyleColor _nodeStyleColorType)
		{
			switch (_nodeStyleColorType)
			{
			case PinStyleColor_Rect:
				return "PinStyleColor_Rect";
			case PinStyleColor_RectBorder:
				return "PinStyleColor_RectBorder";
			}
		}

	};

	struct NodeEditorStyle
	{
		NodeEditorStyleColors colors;

		NodeEditorStyle(ax::NodeEditor::Style& _defaultStyle) :
			colors{ _defaultStyle }
		{

		}

	};
	
#pragma endregion

#pragma region Custom Node Widgets Data
	/*!
	@brief Information to display and interact with custom list boxes inside
			a node.
	@see It is used in ECellEngine::Editor::Utility::NodeEditorDraw::NodeStringListBox
	*/
	struct NodeListBoxStringData
	{
		const std::vector<std::string>* data;
		/*!
		@brief 1-byte char to encode states of the widget
		@details Bit 0 encodes the hovering of an item this frame.

				 Bit 1 encodes the 1 click (selection) of an item this frame.

				 Bit 2 encodes the double click of an item this frame.

				 Bit 3 encodes the hovering of the scroll bar this frame.

				 Bit 4 encodes the activation of the scroll bar this frame.
		*/
		unsigned char utilityState = 0;

		std::size_t doubleClickedItem = 0;

		std::size_t hoveredItem = 0;

		/*!
		@brief The index of the last selected item in the associated node list box.
		*/
		std::size_t selectedItem = 0;

		/*!
		@brief The position from which we start displaying the data of the node
				list box.
		*/
		std::size_t cursor = 0;

		inline bool IsAnItemHovered() noexcept
		{
			return (utilityState >> 0) & 1;
		}

		inline bool IsAnItemClicked() noexcept
		{
			return (utilityState >> 1) & 1;
		}

		inline bool IsAnItemDoubleClicked() noexcept
		{
			return (utilityState >> 2) & 1;
		}

		inline bool IsScrollBarHovered() noexcept
		{
			return (utilityState >> 3) & 1;
		}

		inline bool IsScrollBarActivated() noexcept
		{
			return (utilityState >> 4) & 1;
		}

		inline void SetItemHovered() noexcept
		{
			utilityState |= 1 << 0;
		}
		
		inline void SetItemClicked() noexcept
		{
			utilityState |= 1 << 1;
		}
		
		inline void SetItemDoubleClicked() noexcept
		{
			utilityState |= 1 << 2;
		}
		
		inline void SetScrollBarHovered() noexcept
		{
			utilityState |= 1 << 3;
		}
		
		inline void SetScrollBarActivated() noexcept
		{
			utilityState |= 1 << 4;
		}

		inline void ResetUtilityState() noexcept
		{
			utilityState = 0;
		}
	};
#pragma endregion

#pragma region Node Pins Data
	/*!
	@brief Information about connection between pins.
	@details Note that connection (aka. link) has its own ID. This is useful
			 later with dealing with selections, deletion or other operations.
	@remarks Originally from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct LinkData
	{
		ax::NodeEditor::LinkId id;
		ax::NodeEditor::PinId  startId; //start of link
		ax::NodeEditor::PinId  endId; //target of link

		LinkData(std::size_t& _linkId, ax::NodeEditor::PinId _startId, ax::NodeEditor::PinId _endId) :
			id{ _linkId++ }, startId{ _startId }, endId{ _endId }
		{

		}
	};
	
	/*
	@brief Information about a pin to draw in a node.
	*/
	struct NodePinData
	{
		/*!
		@brief The id of this pin to be identifiable in the node editor.
		*/
		ax::NodeEditor::PinId id;

		/*!
		@brief The name of this pin to display.
		*/
		char* name;

		/*!
		@remarks @p _pinId is incremented immeditely after use.
		*/
		NodePinData(std::size_t& _pinId, char* _name) :
			id{ _pinId++ }, name{ _name }
		{

		}
	};
#pragma endregion

#pragma region Nodes Data
	struct AssetNodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		ax::NodeEditor::NodeId id;

		/*!
		@brief The index of the data in its origin vector/array.
		@details Used to retrieve all the information to be displayed within
				 the node (e.g. name in one of the relevant vectors in
				 ECellEngine::Editor::ModelHierarchyWidget, the actual asset or
				 solver data in ECellEngine::Core::Simulation).
		*/
		std::size_t dataIdx;

		ECellEngine::Data::SBMLModule* data;

		NodePinData outputPins[4];

		unsigned char utilityState = 0;

		NodeListBoxStringData speciesNLB;
		NodeListBoxStringData simpleParametersNLB;
		NodeListBoxStringData computedParametersNLB;
		NodeListBoxStringData reactionsNLB;

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		AssetNodeData(std::size_t& _nodeId, std::size_t _dataIdx, ECellEngine::Data::Module* _data) :
			id{ _nodeId }, dataIdx{ _dataIdx }, data{dynamic_cast<ECellEngine::Data::SBMLModule*>(_data)}
		{
			ax::NodeEditor::SetNodePosition(_nodeId, ImGui::GetIO().MousePos);
			_nodeId++;

			outputPins[0] = NodePinData(_nodeId, "o");
			outputPins[1] = NodePinData(_nodeId, "o");
			outputPins[2] = NodePinData(_nodeId, "o");
			outputPins[3] = NodePinData(_nodeId, "o");

			//Initialize the list boxes data
			speciesNLB.data = &data->GetAllSpecies();
			speciesNLB.cursor = speciesNLB.data->size();

			simpleParametersNLB.data = &data->GetAllSimpleParameter();
			simpleParametersNLB.cursor = simpleParametersNLB.data->size();

			computedParametersNLB.data = &data->GetAllComputedParameter();
			computedParametersNLB.cursor = computedParametersNLB.data->size();

			reactionsNLB.data = &data->GetAllReaction();
			reactionsNLB.cursor = reactionsNLB.data->size();
		}
	};

	struct SolverNodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		ax::NodeEditor::NodeId id;

		/*!
		@brief The index of the data in its origin vector/array.
		@details Used to retrieve all the information to be displayed within
				 the node (e.g. name in one of the relevant vectors in
				 ECellEngine::Editor::ModelHierarchyWidget, the actual asset or
				 solver data in ECellEngine::Core::Simulation).
		*/
		std::size_t dataIdx;

		ECellEngine::Solvers::Solver* data;

		std::vector<NodePinData> outputPins;

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		SolverNodeData(std::size_t& _nodeId, std::size_t _dataIdx, ECellEngine::Solvers::Solver* _data) :
			id{ _nodeId }, dataIdx{ _dataIdx }, data{ _data }
		{
			ax::NodeEditor::SetNodePosition(_nodeId, ImGui::GetIO().MousePos);
			_nodeId++;

			outputPins.push_back(NodePinData(_nodeId, "o"));
		}

	};
#pragma endregion
}