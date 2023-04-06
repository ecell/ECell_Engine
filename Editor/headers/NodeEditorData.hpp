#pragma once
#include "imgui_node_editor.h"

#include "SBMLModule.hpp"


namespace ECellEngine::Editor::Utility
{
enum NodeType
{
	NodeType_Default,

	NodeType_Asset,
	NodeType_Parameter,
	NodeType_Reaction,
	NodeType_Solver,
	NodeType_Species,

	NodeType_Count
};

enum PinType
{
	PinType_Default,

	PinType_Asset,
	PinType_Parameter,
	PinType_Reaction,
	PinType_Solver,
	PinType_Species,

	PinType_Count
};

#pragma region Custom Style
	
	/*enum LinkStyleColor
	{
		LinkStyleColor_BorderHovered,
		LinkStyleColor_BorderSelected,

		LinkStyleColor_Count
	};*/

	enum NodeColorType
	{
		NodeColorType_Bg,
		NodeColorType_Border,
		//NodeColorType_BorderHovered,
		//NodeColorType_BorderSelected,
		NodeColorType_HeaderBg,
		NodeColorType_HeaderActivated,
		NodeColorType_HeaderHovered,

		NodeColorType_Count
	};

	enum PinColorType
	{
		PinColorType_BgActivated,
		PinColorType_BgInactivated,
		PinColorType_Border,

		PinColorType_Count
	};

	struct NodeEditorStyle
	{
		float pinWidth;


		ImVec4 nodeColors[NodeType_Count][NodeColorType_Count];
		ImVec4 pinColors[PinType_Count][PinColorType_Count];

		NodeEditorStyle()
		{
			pinWidth = 8.f;

			nodeColors[NodeType_Default][NodeColorType_Bg] = ImVec4(0.125f, 0.125f, 0.125f, 0.784f);//ax::NodeEditor::StyleColor_NodeBg;
			nodeColors[NodeType_Default][NodeColorType_Border] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			//nodeColors[NodeType_Default][NodeColorType_BorderHovered] = ImVec4(0.196f, 0.690f, 1.000f, 1.000f);//ax::NodeEditor::StyleColor_HovNodeBorder;
			//nodeColors[NodeType_Default][NodeColorType_BorderSelected] = ImVec4(1.000f, 0.690f, 0.196f, 1.000f);//ax::NodeEditor::StyleColor_SelNodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.376f);//ax::NodeEditor::StyleColor_NodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderActivated] = ImVec4(1.000f, 0.690f, 0.196f, 1.000f);//ax::NodeEditor::StyleColor_SelNodeBorder;
			nodeColors[NodeType_Default][NodeColorType_HeaderHovered] = ImVec4(0.196f, 0.690f, 1.000f, 1.000f);//ax::NodeEditor::StyleColor_HovNodeBorder;
			
			nodeColors[NodeType_Asset][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Asset][NodeColorType_Border] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			//nodeColors[NodeType_Asset][NodeColorType_BorderHovered] =		ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Asset][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.25f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Asset][NodeColorType_HeaderHovered] =		ImVec4(0.7f, 0.7f, 0.7f, 0.5f);

			nodeColors[NodeType_Parameter][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Parameter][NodeColorType_Border] = ImVec4(1.f, 0.f, 0.f, 1.f);
			//nodeColors[NodeType_Parameter][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Parameter][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Parameter][NodeColorType_HeaderBg] = ImVec4(1.f, 0.f, 0.f, 1.f);;
			nodeColors[NodeType_Parameter][NodeColorType_HeaderActivated] = ImVec4(1.f, 0.f, 0.f, 1.f);;
			nodeColors[NodeType_Parameter][NodeColorType_HeaderHovered] =	ImVec4(1.f, 0.f, 0.f, 1.f);;

			nodeColors[NodeType_Reaction][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Reaction][NodeColorType_Border] = ImVec4(1.f, 0.f, 0.f, 1.f);
			//nodeColors[NodeType_Reaction][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Reaction][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Reaction][NodeColorType_HeaderBg] = ImVec4(1.f, 0.f, 0.f, 1.f);
			nodeColors[NodeType_Reaction][NodeColorType_HeaderActivated] = ImVec4(1.f, 0.f, 0.f, 1.f);
			nodeColors[NodeType_Reaction][NodeColorType_HeaderHovered] =	ImVec4(1.f, 0.f, 0.f, 1.f);

			nodeColors[NodeType_Solver][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Solver][NodeColorType_Border] = ImVec4(1.f, 0.365f, 0.f, 1.f);
			//nodeColors[NodeType_Solver][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);
			//nodeColors[NodeType_Solver][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Solver][NodeColorType_HeaderBg] = ImVec4(1.f, 0.365f, 0.f, 0.25f);
			nodeColors[NodeType_Solver][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Solver][NodeColorType_HeaderHovered] =		ImVec4( 1.f, 0.365f,  0.f,  0.5f);

			nodeColors[NodeType_Species][NodeColorType_Bg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Species][NodeColorType_Border] = ImVec4(1.f, 0.f, 0.f, 1.f);
			//nodeColors[NodeType_Species][NodeColorType_BorderHovered] =	ImVec4(0.f, 1.f, 0.f, 1.f);.f);
			//nodeColors[NodeType_Species][NodeColorType_BorderSelected] =	ImVec4(0.f, 0.f, 1.f, 1.f);
			nodeColors[NodeType_Species][NodeColorType_HeaderBg] = ImVec4(1.f, 0.f, 0.f, 0.25f);
			nodeColors[NodeType_Species][NodeColorType_HeaderActivated] = ImVec4(0.f, 0.f, 0.f, 0.5f);
			nodeColors[NodeType_Species][NodeColorType_HeaderHovered] =		ImVec4( 1.f,  0.f,  0.f, 0.5f);

			pinColors[PinType_Default][PinColorType_BgActivated] = nodeColors[NodeType_Default][NodeColorType_Border];
			pinColors[PinType_Default][PinColorType_BgInactivated] = nodeColors[NodeType_Default][NodeColorType_Bg];
			pinColors[PinType_Default][PinColorType_Border] = nodeColors[NodeType_Default][NodeColorType_Border];

			pinColors[PinType_Asset][PinColorType_BgActivated] = nodeColors[NodeType_Asset][NodeColorType_Border];
			pinColors[PinType_Asset][PinColorType_BgInactivated] = nodeColors[NodeType_Asset][NodeColorType_Bg];
			pinColors[PinType_Asset][PinColorType_Border] = nodeColors[NodeType_Asset][NodeColorType_Border];

			pinColors[PinType_Parameter][PinColorType_BgActivated] = nodeColors[NodeType_Parameter][NodeColorType_Border];
			pinColors[PinType_Parameter][PinColorType_BgInactivated] = nodeColors[NodeType_Parameter][NodeColorType_Bg];
			pinColors[PinType_Parameter][PinColorType_Border] = nodeColors[NodeType_Parameter][NodeColorType_Border];

			pinColors[PinType_Reaction][PinColorType_BgActivated] = nodeColors[NodeType_Reaction][NodeColorType_Border];
			pinColors[PinType_Reaction][PinColorType_BgInactivated] = nodeColors[NodeType_Reaction][NodeColorType_Bg];
			pinColors[PinType_Reaction][PinColorType_Border] = nodeColors[NodeType_Reaction][NodeColorType_Border];

			pinColors[PinType_Solver][PinColorType_BgActivated] = nodeColors[NodeType_Solver][NodeColorType_Border];
			pinColors[PinType_Solver][PinColorType_BgInactivated] = nodeColors[NodeType_Solver][NodeColorType_Bg];
			pinColors[PinType_Solver][PinColorType_Border] = nodeColors[NodeType_Solver][NodeColorType_Border];
			
			pinColors[PinType_Species][PinColorType_BgActivated] = nodeColors[NodeType_Species][NodeColorType_Border];
			pinColors[PinType_Species][PinColorType_BgInactivated] = nodeColors[NodeType_Species][NodeColorType_Bg];
			pinColors[PinType_Species][PinColorType_Border] = nodeColors[NodeType_Species][NodeColorType_Border];

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

#pragma region Node Pins & Links Data
	/*!
	@brief Information about connection between pins.
	@details Note that connection (aka. link) has its own ID. This is useful
			 later with dealing with selections, deletion or other operations.
	@remarks Originally from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct LinkData
	{
		ax::NodeEditor::LinkId id;
		ax::NodeEditor::PinId startIds[2];
		ax::NodeEditor::PinId endIds[2];

		LinkData(std::size_t& _linkId, ax::NodeEditor::PinId _startId, ax::NodeEditor::PinId _endId) :
			id{ _linkId++ }, startIds{ _startId, _startId }, endIds{ _endId, _endId }
		{

		}

		/*!
		@brief Sets the PinId @p _id as a candidate to be the start of the link
				with a set @p _priority.
		@param _id The new pin id to set as the start of this link.
		@param _priority MUST be of value 0 or 1. It describes if this id should
				be used as the first or second choice when trying to draw a link
				from this ::LinKData
		*/
		inline void OverrideStartFallbackPin(ax::NodeEditor::PinId _id, unsigned short _priority)
		{
			startIds[_priority] = _id;
		}

		/*!
		@brief Sets the PinId @p _id as a candidate to be the end of the link
				with a set @p _priority.
		@param _id The new pin id to set as the end of this link.
		@param _priority MUST be of value 0 or 1. It describes if this id should
				be used as the first or second choice when trying to draw a link
				from this ::LinKData
		*/
		inline void OverrideEndFallbackPin(ax::NodeEditor::PinId _id, unsigned short _priority)
		{
			endIds[_priority] = _id;
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
		@brief Whether a link is connected to the pin.
		*/
		bool isUsed = false;

		NodePinData() = default;

		/*!
		@remarks @p _pinId is incremented immeditely after use.
		*/
		NodePinData(std::size_t& _pinId) :
			id{ _pinId++ }
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

		NodePinData inputPin; // only for the Solver link
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

			inputPin = NodePinData(_nodeId);

			outputPins[0] = NodePinData(_nodeId);
			outputPins[1] = NodePinData(_nodeId);
			outputPins[2] = NodePinData(_nodeId);
			outputPins[3] = NodePinData(_nodeId);

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

		NodePinData outputPin;

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		SolverNodeData(std::size_t& _nodeId, std::size_t _dataIdx, ECellEngine::Solvers::Solver* _data) :
			id{ _nodeId }, dataIdx{ _dataIdx }, data{ _data }
		{
			ax::NodeEditor::SetNodePosition(_nodeId, ImGui::GetIO().MousePos);
			_nodeId++;

			outputPin = NodePinData(_nodeId);
		}

	};

	struct SpeciesNodeData 
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

		ECellEngine::Data::Species* data;

		NodePinData inputPins[8];
		NodePinData outputPins[7];

		unsigned char utilityState = 0;

		SpeciesNodeData(std::size_t& _nodeId, std::size_t _dataIdx, ECellEngine::Data::Species* _data) :
			id{ _nodeId }, dataIdx{ _dataIdx }, data{ _data }
		{
			ax::NodeEditor::SetNodePosition(_nodeId, ImVec2(300.f+ImGui::GetIO().MousePos.x, 0.f+ ImGui::GetIO().MousePos.y));
			_nodeId++;

			//Collapsing Header Model Links
			inputPins[0] = NodePinData(_nodeId);
			outputPins[0] = NodePinData(_nodeId);

			//Asset
			inputPins[1] = NodePinData(_nodeId);

			//Computed parameters equation
			inputPins[2] = NodePinData(_nodeId);
			outputPins[1] = NodePinData(_nodeId);

			//Reactions's Reactants
			inputPins[3] = NodePinData(_nodeId);
			outputPins[2] = NodePinData(_nodeId);

			//Reaction's Products
			inputPins[4] = NodePinData(_nodeId);
			outputPins[3] = NodePinData(_nodeId);

			//Reaction's Kinetic Law
			inputPins[5] = NodePinData(_nodeId);
			outputPins[4] = NodePinData(_nodeId);

			//Collapsing Header Data Fields
			inputPins[6] = NodePinData(_nodeId);
			outputPins[5] = NodePinData(_nodeId);

			//Quantity
			inputPins[7] = NodePinData(_nodeId);
			outputPins[6] = NodePinData(_nodeId);
		}
	};

#pragma endregion

#pragma region Data Global API

	struct ModelNodeBasedViewerContext
	{
		std::size_t uniqueId;

		NodeEditorStyle style;

	};

	ModelNodeBasedViewerContext* GetCurrentMNBVContext();

	ImVec4* GetNodeColors(NodeType _nodeType);

	ImVec4* GetPinColors(PinType _nodeType);

	/*inline char* GetLinkStyleColorName(LinkStyleColor _linkStyleColorType)
	{
		switch (_linkStyleColorType)
		{
		case LinkStyleColor_BorderHovered:
			return "LinkStyleColor_BorderHovered";
		case LinkStyleColor_BorderSelected:
			return "LinkStyleColor_BorderSelected";
		}
	}*/

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

	NodeEditorStyle* GetMNBVSyle();

	void SetCurrentMNBVContext(ModelNodeBasedViewerContext* _ctxt);

#pragma endregion


}