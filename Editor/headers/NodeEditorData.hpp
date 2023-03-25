#pragma once
#include "imgui_node_editor.h"

#include "Module.hpp"


namespace ECellEngine::Editor::Utility
{
	/*!
	@brief Struct to hold basic information about connection between pins.
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

	/*
	@brief Information about a set of pins to draw in a node
	@remarks The number of strings encoded in ::pinNames MUST
				 be identical to ::nbPins. If ::pinNames <
				 ::nbPins, an index error will be generated. If
				 If ::pinNames > ::nbPins no errors will be
				 generated but not every pins will be drawn.
	*/
	struct NodePinsData
	{
		/*!
		@brief The number of input pins to add to the node.
		*/
		unsigned short nbPins;

		/*!
		@brief The array of names of each pin to draw.
		*/
		std::vector<char*> pinNames;
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

		ECellEngine::Data::Module* data;

		NodePinsData inputPins;
		NodePinsData outputPins;

		AssetNodeData(std::size_t& _nodeId, std::size_t _dataIdx) :
			id{ _nodeId }, dataIdx{ _dataIdx }
		{
			inputPins.nbPins = 3;
			inputPins.pinNames = { "o", "o", "o" };

			outputPins.nbPins = 3;
			outputPins.pinNames = { "o", "o", "o" };
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

		NodePinsData inputPins;
		NodePinsData outputPins;

		SolverNodeData(std::size_t& _nodeId, std::size_t _dataIdx) :
			id{ _nodeId }, dataIdx{ _dataIdx }
		{
			inputPins.nbPins = 0;

			outputPins.nbPins = 1;
			outputPins.pinNames = { "o" };
		}

	};
}