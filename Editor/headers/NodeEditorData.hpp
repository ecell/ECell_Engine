#pragma once
#include "imgui_node_editor.h"

#include "SBMLModule.hpp"


namespace ECellEngine::Editor::Utility
{
#pragma region Custom Node Widgets Data
	@brief Information about connection between pins.
	@details Note that connection (aka. link) has its own ID. This is useful
			 later with dealing with selections, deletion or other operations.
	@remarks Originally from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct LinkData
	{
		ax::NodeEditor::LinkId id;
		ax::NodeEditor::PinId  inputId;
		ax::NodeEditor::PinId  outputId;
	};

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

		std::vector<NodePinData> inputPins;
		std::vector<NodePinData> outputPins;

		unsigned char utilityState = 0;

		NodeListBoxStringData speciesNLB;
		NodeListBoxStringData simpleParametersNLB;
		NodeListBoxStringData computedParametersNLB;
		NodeListBoxStringData reactionsNLB;

		/*!
		@remarks @p _nodeId is incremented immeditely after use.
		*/
		AssetNodeData(std::size_t& _nodeId, std::size_t _dataIdx, ECellEngine::Data::Module* _data) :
			id{ _nodeId }, dataIdx{ _dataIdx }, data{dynamic_cast<ECellEngine::Data::SBMLModule*>(_data)}
		{
			ax::NodeEditor::SetNodePosition(_nodeId, ImGui::GetIO().MousePos);
			_nodeId++;

			inputPins.push_back(NodePinData(_nodeId, "o"));
			inputPins.push_back(NodePinData(_nodeId, "o"));
			inputPins.push_back(NodePinData(_nodeId, "o"));
			inputPins.push_back(NodePinData(_nodeId, "o"));
			//Initialize the list boxes data
			speciesNLB.data = &data->GetAllSpecies();
			speciesNLB.cursor = speciesNLB.data->size();

			simpleParametersNLB.data = &data->GetAllSimpleParameter();
			simpleParametersNLB.cursor = simpleParametersNLB.data->size();

			computedParametersNLB.data = &data->GetAllComputedParameter();
			computedParametersNLB.cursor = computedParametersNLB.data->size();

			reactionsNLB.data = &data->GetAllReaction();
			reactionsNLB.cursor = reactionsNLB.data->size();
			simpleParametersNLB.cursor = data->GetAllSimpleParameter().size();
			computedParametersNLB.cursor = data->GetAllComputedParameter().size();
			reactionsNLB.cursor = data->GetAllReaction().size();
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
		@remarks @p _nodeId is incremented immeditely after use.
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