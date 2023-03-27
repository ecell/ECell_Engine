#pragma once
#include "imgui_node_editor.h"

#include "SBMLModule.hpp"


namespace ECellEngine::Editor::Utility
{
	/*!
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
	struct NodeListBoxData
	{
		/*!
		@brief The index of the last selected item in the associated node list box.
		*/
		std::size_t selectedItemIdx = 0;

		/*!
		@brief The position from which we start displaying the data of the node 
				list box.
		*/
		std::size_t cursor = 0;
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

		NodeListBoxData speciesNLB;
		NodeListBoxData simpleParametersNLB;
		NodeListBoxData computedParametersNLB;
		NodeListBoxData reactionsNLB;

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

			outputPins.push_back(NodePinData(_nodeId, "o"));
			outputPins.push_back(NodePinData(_nodeId, "o"));
			outputPins.push_back(NodePinData(_nodeId, "o"));
			outputPins.push_back(NodePinData(_nodeId, "o"));

			//Initialize the list boxes cursors
			speciesNLB.cursor = data->GetAllSpecies().size();
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
}