#pragma once
#include "imgui_node_editor.h"

#include "SBMLModule.hpp"


namespace ECellEngine::Editor::Utility
{
#pragma region Custom Node Widgets Data
	/*!
	@brief Information to display and interact with custom list boxes inside
			a node.
	@see It is used in ECellEngine::Editor::Utility::NodeEditorDraw::NodeStringListBox
	*/
	struct NodeListBoxStringData
	{
		const std::vector<std::string>* data = nullptr;
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

		std::size_t scrollBarID = 0;

		NodeListBoxStringData() = default;

		NodeListBoxStringData(const std::vector<std::string>* _data,  std::size_t _scrollBarId) :
			data{_data}, cursor{ _data->size()}, scrollBarID{_scrollBarId}
		{

		}

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

		LinkData(ax::NodeEditor::PinId _startId, ax::NodeEditor::PinId _endId) :
			id{ GetMNBVCtxtNextId()}, startIds{_startId, _startId}, endIds{_endId, _endId}
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
			id{ _pinId }
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
		//std::size_t dataIdx;

		ECellEngine::Data::SBMLModule* data;

		NodePinData inputPin; // only for the Solver link
		NodePinData outputPins[4];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[4];

		NodeListBoxStringData speciesNLB;
		NodeListBoxStringData simpleParametersNLB;
		NodeListBoxStringData computedParametersNLB;
		NodeListBoxStringData reactionsNLB;

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		AssetNodeData(ECellEngine::Data::Module* _data) :
			id{ GetMNBVCtxtNextId() }, data{dynamic_cast<ECellEngine::Data::SBMLModule*>(_data)}
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			//Solver
			inputPin = NodePinData(GetMNBVCtxtNextId());

			//Species Collapsing header
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();
			outputPins[0] = NodePinData(GetMNBVCtxtNextId());

			//Simple Parameters Collapsing header
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();
			outputPins[1] = NodePinData(GetMNBVCtxtNextId());

			//Computed Parameters Collapsing header
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			outputPins[2] = NodePinData(GetMNBVCtxtNextId());

			//Reactions Collapsing header
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();
			outputPins[3] = NodePinData(GetMNBVCtxtNextId());

			//Initialize the list boxes data
			speciesNLB = { &data->GetAllSpecies(), GetMNBVCtxtNextId()};
			simpleParametersNLB = { &data->GetAllSimpleParameter(), GetMNBVCtxtNextId()};
			computedParametersNLB = { &data->GetAllComputedParameter(), GetMNBVCtxtNextId()};
			reactionsNLB = { &data->GetAllReaction(), GetMNBVCtxtNextId()};
		}
	};

	struct ComputedParameterNodeData
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
		//std::size_t dataIdx;

		ECellEngine::Data::ComputedParameter* data;

		NodePinData inputPins[10];
		NodePinData outputPins[9];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[5];

		/*!
		@biref The array of of list box data that are potentially displayed in
				this node.
		@details At index 0 we find the list box with the links to other computed
				 parameters where this computed parameter is also found.

				 At index 1 we find the list box with the links to reaction's
				 kinetic laws where this computed parameter is found.

				 At index 2 we find the list box with the links to species that
				 are involved (as quantities) in the computation of this parameter.

				 At index 3 we find the list box with the links to simple 
				 parameters that are involved (as values) in the computation of
				 this parameter.

				 At index 4 we find the list box with the links to computed
				 parameters that are involved (as values) in the computation of
				 this parameter.
		*/
		NodeListBoxStringData nslbData[5];
		std::vector<std::string> speciesOperands;
		std::vector<std::string> simpleParametersOperands;
		std::vector<std::string> computedParametersOperands;

		ComputedParameterNodeData(const ComputedParameterNodeData& _cpnd):
			id{_cpnd.id}, data{_cpnd.data},
			inputPins{_cpnd.inputPins[0], _cpnd.inputPins[1] , _cpnd.inputPins[2] ,
					  _cpnd.inputPins[3] , _cpnd.inputPins[4] , _cpnd.inputPins[5],
					  _cpnd.inputPins[6] , _cpnd.inputPins[7] , _cpnd.inputPins[8], _cpnd.inputPins[9] },
			outputPins{ _cpnd.outputPins[0], _cpnd.outputPins[1] , _cpnd.outputPins[2] ,
					  _cpnd.outputPins[3] , _cpnd.outputPins[4] , _cpnd.outputPins[5],
					  _cpnd.outputPins[6] , _cpnd.outputPins[7] , _cpnd.outputPins[8] },
			utilityState{_cpnd.utilityState},
			collapsingHeadersIds{ _cpnd.collapsingHeadersIds[0], _cpnd.collapsingHeadersIds[1] , _cpnd.collapsingHeadersIds[2] ,
					  _cpnd.collapsingHeadersIds[3] , _cpnd.collapsingHeadersIds[4] },
			nslbData{ _cpnd.nslbData[0], _cpnd.nslbData[1] , _cpnd.nslbData[2] ,
					  _cpnd.nslbData[3] , _cpnd.nslbData[4] },
			speciesOperands{_cpnd.speciesOperands },
			simpleParametersOperands{_cpnd.simpleParametersOperands},
			computedParametersOperands{ _cpnd.computedParametersOperands }
		{
			nslbData[2].data = &speciesOperands;
			nslbData[3].data = &simpleParametersOperands;
			nslbData[4].data = &computedParametersOperands;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		ComputedParameterNodeData(ECellEngine::Data::ComputedParameter* _data) :
			id{ GetMNBVCtxtNextId() }, data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			//ModelLinks Collapsing header
			inputPins[0] = NodePinData(GetMNBVCtxtNextId());
			outputPins[0] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();

			//Asset
			inputPins[1] = NodePinData(GetMNBVCtxtNextId());

			//Computed Parameters section
			inputPins[2] = NodePinData(GetMNBVCtxtNextId());
			outputPins[1] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();
			//lbsData[0] = { /*list of computed parameters where this one appears*/, GetMNBVCtxtNextId()}

			//Kinetic Laws section
			inputPins[3] = NodePinData(GetMNBVCtxtNextId());
			outputPins[2] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			//lbsData[1] = { /*list of kinetic laws where this one appears*/, GetMNBVCtxtNextId()}

			//Data Fields collapsing header
			inputPins[4] = NodePinData(GetMNBVCtxtNextId());
			outputPins[3] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();

			//Equation Operands collapsing header
			inputPins[5] = NodePinData(GetMNBVCtxtNextId());
			outputPins[4] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[4] = GetMNBVCtxtNextId();

			//Node String List Box for Species Operands
			inputPins[6] = NodePinData(GetMNBVCtxtNextId());
			outputPins[5] = NodePinData(GetMNBVCtxtNextId());
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);
			nslbData[2] = { &speciesOperands, GetMNBVCtxtNextId() };
			
			//Node String List Box for Simple Parameter Operands
			inputPins[7] = NodePinData(GetMNBVCtxtNextId());
			outputPins[6] = NodePinData(GetMNBVCtxtNextId());
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);
			nslbData[3] = { &simpleParametersOperands, GetMNBVCtxtNextId() };
			
			//Node String List Box for Computed Parameter Operands
			inputPins[8] = NodePinData(GetMNBVCtxtNextId());
			outputPins[7] = NodePinData(GetMNBVCtxtNextId());
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);
			nslbData[4] = { &computedParametersOperands, GetMNBVCtxtNextId() };
			
			//Value Float string of the computation of the formula
			//The user cannot directly change the value of the result
			inputPins[9] = NodePinData(GetMNBVCtxtNextId());
			outputPins[8] = NodePinData(GetMNBVCtxtNextId());
		}
	};

	struct ReactionNodeData
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
		//std::size_t dataIdx;

		ECellEngine::Data::Reaction* data;

		NodePinData inputPins[10];
		NodePinData outputPins[9];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[5];

		/*!
		@biref The array of of list box data that are potentially displayed in
				this node.
		@details At index 0 we find the list box with the links to the reactants
				 species of this reaction.

				 At index 1 we find the list box with the links to the products
				 species of this reaction.

				 At index 2 we find the list box with the links to species that
				 are involved (as quantities) in the computation of the kinetic
				 law.

				 At index 3 we find the list box with the links to simple
				 parameters that are involved (as values) in the computation
				 of the kinetic law.

				 At index 4 we find the list box with the links to computed
				 parameters that are involved (as values) in the computation
				 of the kinetic law.
		*/
		NodeListBoxStringData nslbData[5];
		std::vector<std::string> speciesOperands;
		std::vector<std::string> simpleParametersOperands;
		std::vector<std::string> computedParametersOperands;

		ReactionNodeData(const ReactionNodeData& _cpnd) :
			id{ _cpnd.id }, data{ _cpnd.data },
			inputPins{ _cpnd.inputPins[0], _cpnd.inputPins[1] , _cpnd.inputPins[2] ,
					  _cpnd.inputPins[3] , _cpnd.inputPins[4] , _cpnd.inputPins[5],
					  _cpnd.inputPins[6] , _cpnd.inputPins[7] , _cpnd.inputPins[8], _cpnd.inputPins[9] },
			outputPins{ _cpnd.outputPins[0], _cpnd.outputPins[1] , _cpnd.outputPins[2] ,
					  _cpnd.outputPins[3] , _cpnd.outputPins[4] , _cpnd.outputPins[5],
					  _cpnd.outputPins[6] , _cpnd.outputPins[7] , _cpnd.outputPins[8] },
			utilityState{ _cpnd.utilityState },
			collapsingHeadersIds{ _cpnd.collapsingHeadersIds[0], _cpnd.collapsingHeadersIds[1] , _cpnd.collapsingHeadersIds[2] ,
					  _cpnd.collapsingHeadersIds[3] , _cpnd.collapsingHeadersIds[4] },
			nslbData{ _cpnd.nslbData[0], _cpnd.nslbData[1] , _cpnd.nslbData[2] ,
					  _cpnd.nslbData[3] , _cpnd.nslbData[4] },
			speciesOperands{ _cpnd.speciesOperands },
			simpleParametersOperands{ _cpnd.simpleParametersOperands },
			computedParametersOperands{ _cpnd.computedParametersOperands }
		{
			nslbData[2].data = &speciesOperands;
			nslbData[3].data = &simpleParametersOperands;
			nslbData[4].data = &computedParametersOperands;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		ReactionNodeData(ECellEngine::Data::Reaction* _data) :
			id{ GetMNBVCtxtNextId() },  data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			//ModelLinks Collapsing header
			inputPins[0] = NodePinData(GetMNBVCtxtNextId());
			outputPins[0] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();

			//Asset
			inputPins[1] = NodePinData(GetMNBVCtxtNextId());

			//Reactants section
			inputPins[2] = NodePinData(GetMNBVCtxtNextId());
			outputPins[1] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();
			nslbData[0] = { &_data->GetReactants() , GetMNBVCtxtNextId() };

			//Products section
			inputPins[3] = NodePinData(GetMNBVCtxtNextId());
			outputPins[2] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			nslbData[1] = { &data->GetProducts(), GetMNBVCtxtNextId() };

			//Kinetic Law collapsing header
			inputPins[4] = NodePinData(GetMNBVCtxtNextId());
			outputPins[3] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();

			//Kinetic Law Operands collapsing header
			inputPins[5] = NodePinData(GetMNBVCtxtNextId());
			outputPins[4] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[4] = GetMNBVCtxtNextId();

			//Node String List Box for Species Operands, from Kinetic Law
			inputPins[6] = NodePinData(GetMNBVCtxtNextId());
			outputPins[5] = NodePinData(GetMNBVCtxtNextId());
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);
			nslbData[2] = { &speciesOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Simple Parameter Operands, from Kinetic Law
			inputPins[7] = NodePinData(GetMNBVCtxtNextId());
			outputPins[6] = NodePinData(GetMNBVCtxtNextId());
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);
			nslbData[3] = { &simpleParametersOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Computed Parameter Operands, from Kinetic Law
			inputPins[8] = NodePinData(GetMNBVCtxtNextId());
			outputPins[7] = NodePinData(GetMNBVCtxtNextId());
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);
			nslbData[4] = { &computedParametersOperands, GetMNBVCtxtNextId() };

			//Kinetic Law Value Float field (must be Read Only)
			inputPins[9] = NodePinData(GetMNBVCtxtNextId());
			outputPins[8] = NodePinData(GetMNBVCtxtNextId());
		}
	};

	struct SimpleParameterNodeData
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

		ECellEngine::Data::SimpleParameter* data;

		NodePinData inputPins[6];
		NodePinData outputPins[5];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[4];

		NodeListBoxStringData nslbData[2];

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		SimpleParameterNodeData(ECellEngine::Data::SimpleParameter* _data) :
			id{ GetMNBVCtxtNextId() }, data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			//ModelLinks Collapsing header
			inputPins[0] = NodePinData(GetMNBVCtxtNextId());
			outputPins[0] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();

			//Asset
			inputPins[1] = NodePinData(GetMNBVCtxtNextId());

			//Computed Parameters section
			inputPins[2] = NodePinData(GetMNBVCtxtNextId());
			outputPins[1] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();
			//lbsData[0] = { /*list of computed parameters where this one appears*/, GetMNBVCtxtNextId()}

			//Kinetic Laws section
			inputPins[3] = NodePinData(GetMNBVCtxtNextId());
			outputPins[2] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			//lbsData[1] = { /*list of kinetic laws where this one appears*/, GetMNBVCtxtNextId()}
			
			//Data Fields collapsing header
			inputPins[4] = NodePinData(GetMNBVCtxtNextId());
			outputPins[3] = NodePinData(GetMNBVCtxtNextId());
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();
			
			//Value Float field
			inputPins[5] = NodePinData(GetMNBVCtxtNextId());
			outputPins[4] = NodePinData(GetMNBVCtxtNextId());
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
		//std::size_t dataIdx;

		ECellEngine::Solvers::Solver* data;

		NodePinData outputPin;

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		SolverNodeData(ECellEngine::Solvers::Solver* _data) :
			id{ GetMNBVCtxtNextId() }, data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			outputPin = NodePinData(GetMNBVCtxtNextId());
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
		//std::size_t dataIdx;

		ECellEngine::Data::Species* data;

		NodePinData inputPins[8];
		NodePinData outputPins[7];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[2];

		SpeciesNodeData(ECellEngine::Data::Species* _data) :
			id{ GetMNBVCtxtNextId() }, data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f+ImGui::GetIO().MousePos.x, 0.f+ ImGui::GetIO().MousePos.y));

			//Id to uniquely identify the collapsing header for the MODEL LINKS of each species node
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();
			//Id to uniquely identify the collapsing header for the DATA FIELDS of each species node
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();

			//Collapsing Header Model Links
			inputPins[0] = NodePinData(GetMNBVCtxtNextId());
			outputPins[0] = NodePinData(GetMNBVCtxtNextId());

			//Asset
			inputPins[1] = NodePinData(GetMNBVCtxtNextId());

			//Computed parameters equation
			inputPins[2] = NodePinData(GetMNBVCtxtNextId());
			outputPins[1] = NodePinData(GetMNBVCtxtNextId());

			//Reactions's Reactants
			inputPins[3] = NodePinData(GetMNBVCtxtNextId());
			outputPins[2] = NodePinData(GetMNBVCtxtNextId());

			//Reaction's Products
			inputPins[4] = NodePinData(GetMNBVCtxtNextId());
			outputPins[3] = NodePinData(GetMNBVCtxtNextId());

			//Reaction's Kinetic Law
			inputPins[5] = NodePinData(GetMNBVCtxtNextId());
			outputPins[4] = NodePinData(GetMNBVCtxtNextId());

			//Collapsing Header Data Fields
			inputPins[6] = NodePinData(GetMNBVCtxtNextId());
			outputPins[5] = NodePinData(GetMNBVCtxtNextId());

			//Quantity
			inputPins[7] = NodePinData(GetMNBVCtxtNextId());
			outputPins[6] = NodePinData(GetMNBVCtxtNextId());
		}
	};

#pragma endregion
}