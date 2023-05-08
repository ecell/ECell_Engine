#pragma once
#include "implot.h"
#include "imgui_node_editor.h"

#include "BinaryOperatedVector.hpp"
#include "Timer.hpp"
#include "ModelNodeBasedViewerGlobal.hpp"
#include "PlotUtility.hpp"

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

#pragma region Base Node, Pin & Link
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
			id{ GetMNBVCtxtNextId() }, startIds{_startId, _startId}, endIds{_endId, _endId}
		{

		}

		inline operator std::size_t() { return (std::size_t)id; }

		friend inline bool operator==(const LinkData& lhs, const LinkData& rhs) { return (std::size_t)lhs.id == (std::size_t)rhs.id; }
		friend inline bool operator!=(const LinkData& lhs, const LinkData& rhs) { return !(lhs == rhs); }

		friend inline bool operator< (const LinkData& lhs, const LinkData& rhs) { return (std::size_t)lhs.id < (std::size_t)rhs.id; }
		friend inline bool operator> (const LinkData& lhs, const LinkData& rhs) { return rhs < lhs; }
		friend inline bool operator<=(const LinkData& lhs, const LinkData& rhs) { return !(lhs > rhs); }
		friend inline bool operator>=(const LinkData& lhs, const LinkData& rhs) { return !(lhs < rhs); }

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

	struct NodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		ax::NodeEditor::NodeId id;

		NodeData() :
			id{ GetMNBVCtxtNextId() }
		{

		}

		NodeData(const NodeData& _nd) :
			id{ _nd.id }
		{

		}

		inline operator std::size_t() { return (std::size_t)id; }

		friend inline bool operator==(const NodeData& lhs, const NodeData& rhs) { return (std::size_t)lhs.id == (std::size_t)rhs.id; }
		friend inline bool operator!=(const NodeData& lhs, const NodeData& rhs) { return !(lhs == rhs); }

		friend inline bool operator< (const NodeData& lhs, const NodeData& rhs) { return (std::size_t)lhs.id < (std::size_t)rhs.id; }
		friend inline bool operator> (const NodeData& lhs, const NodeData& rhs) { return rhs < lhs; }
		friend inline bool operator<=(const NodeData& lhs, const NodeData& rhs) { return !(lhs > rhs); }
		friend inline bool operator>=(const NodeData& lhs, const NodeData& rhs) { return !(lhs < rhs); }

		virtual void InputConnect(std::size_t _nodeInputPinId) = 0;
		
		virtual void InputUpdate(std::size_t _nodeInputPinId, char* _data) = 0;

		virtual void InputUpdate(std::size_t _nodeInputPinId, float _data) = 0;

		virtual void OutputConnect(std::size_t _nodeOutputPinId) = 0;

		virtual void OutputUpdate(std::size_t _nodeOutputPinId) = 0;
	};

	/*
	@brief Information about a pin to draw in a node.
	*/
	struct NodePinData
	{
		/*!
		@brief The id of this pin to be identifiable in the node editor.
		*/
		ax::NodeEditor::PinId id = 0;

		ax::NodeEditor::PinKind kind = ax::NodeEditor::PinKind::Input;

		PinType type = PinType_Default;

		NodeData* node = nullptr;

		/*!
		@brief Whether a link is connected to the pin.
		*/
		bool isUsed = false;

		NodePinData() = default;

		/*!
		@remarks @p _pinId is incremented immeditely after use.
		*/
		NodePinData(std::size_t& _pinId, ax::NodeEditor::PinKind _kind, PinType _type, NodeData* _node) :
			id{ _pinId }, kind{ _kind }, type { _type }, node{ _node }
		{

		}

		inline operator std::size_t() { return (std::size_t)id; }

		friend inline bool operator==(const NodePinData& lhs, const NodePinData& rhs) { return (std::size_t)lhs.id == (std::size_t)rhs.id; }
		friend inline bool operator!=(const NodePinData& lhs, const NodePinData& rhs) { return !(lhs == rhs); }

		friend inline bool operator< (const NodePinData& lhs, const NodePinData& rhs) { return (std::size_t)lhs.id < (std::size_t)rhs.id; }
		friend inline bool operator> (const NodePinData& lhs, const NodePinData& rhs) { return rhs < lhs; }
		friend inline bool operator<=(const NodePinData& lhs, const NodePinData& rhs) { return !(lhs > rhs); }
		friend inline bool operator>=(const NodePinData& lhs, const NodePinData& rhs) { return !(lhs < rhs); }
	};
#pragma endregion

#pragma region Derived Pin Data
	struct NodeInputPinData : public NodePinData
	{
		NodeInputPinData() = default;

		NodeInputPinData(std::size_t& _pinId, PinType _type, NodeData* _node):
			NodePinData( _pinId, ax::NodeEditor::PinKind::Input, _type, _node)
		{

		}

		template<class Data>
		inline void Receive(Data _data)
		{
			node->InputUpdate((std::size_t)id, _data);
		}
	};

	struct NodeOutputPinData : public NodePinData
	{
		std::vector<NodeInputPinData*> subscribers;
		
		NodeOutputPinData() = default;

		NodeOutputPinData(std::size_t& _pinId, PinType _type, NodeData* _node):
			NodePinData( _pinId, ax::NodeEditor::PinKind::Output, _type, _node)
		{

		}

		inline void AddSubscriber(NodeInputPinData* _newSubscriber)
		{
			subscribers.push_back(_newSubscriber);

			//What to do on connection.
			node->OutputConnect((std::size_t)id);
			_newSubscriber->node->InputConnect((std::size_t)_newSubscriber->id);

			//Send data for the first time
			node->OutputUpdate((std::size_t)id);
		}

		template<class Data>
		inline void Broadcast(Data _data)
		{
			for (std::vector<NodeInputPinData*>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
			{
				(*it)->Receive(_data);
			}
		}
	};

#pragma endregion

#pragma region Derived Nodes Data

	struct AssetNodeData : public NodeData
	{
		ECellEngine::Data::SBMLModule* data;

		NodeInputPinData inputPins[1];
		NodeOutputPinData outputPins[4];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[4];

		NodeListBoxStringData speciesNLB;
		NodeListBoxStringData simpleParametersNLB;
		NodeListBoxStringData computedParametersNLB;
		NodeListBoxStringData reactionsNLB;

		AssetNodeData(const AssetNodeData& _and) :
			NodeData(_and), data{ _and.data },
			inputPins{ _and.inputPins[0] },
			outputPins{ _and.outputPins[0], _and.outputPins[1] , _and.outputPins[2] ,
					  _and.outputPins[3] },
			utilityState{ _and.utilityState },
			collapsingHeadersIds{ _and.collapsingHeadersIds[0], _and.collapsingHeadersIds[1] , _and.collapsingHeadersIds[2] ,
					  _and.collapsingHeadersIds[3] },
			speciesNLB{ _and.speciesNLB }, simpleParametersNLB{ _and.simpleParametersNLB },
			computedParametersNLB{ _and.computedParametersNLB }, reactionsNLB{ _and.reactionsNLB }
		{
			inputPins[0].node = this;

			outputPins[0].node = this;
			outputPins[1].node = this;
			outputPins[2].node = this;
			outputPins[3].node = this;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		AssetNodeData(ECellEngine::Data::Module* _data) :
			NodeData(), data{dynamic_cast<ECellEngine::Data::SBMLModule*>(_data)}
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Solver, this);//Solver
			
			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);//Species Collapsing header
			outputPins[1] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Simple Parameters Collapsing header
			outputPins[2] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters Collapsing header
			outputPins[3] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Reaction, this);//Reactions Collapsing header
			
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();//Species Collapsing header
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();//Simple Parameters Collapsing header
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();//Computed Parameters Collapsing header
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();//Reactions Collapsing header

			//Initialize the list boxes data
			speciesNLB = { &data->GetAllSpecies(), GetMNBVCtxtNextId() };
			simpleParametersNLB = { &data->GetAllSimpleParameter(), GetMNBVCtxtNextId() };
			computedParametersNLB = { &data->GetAllComputedParameter(), GetMNBVCtxtNextId() };
			reactionsNLB = { &data->GetAllReaction(), GetMNBVCtxtNextId() };

		}

		void InputConnect(std::size_t _nodeInputPinId) override {};//not used in asset node data

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override;

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override {};//not used in asset node data

		void OutputConnect(std::size_t _nodeOutputPinId) override {};//not used in asset node data

		void OutputUpdate(std::size_t _nodeOutputPinId) override {};//not used in asset node data
	};

	struct ComputedParameterNodeData : public NodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		//ax::NodeEditor::NodeId id;

		ECellEngine::Data::ComputedParameter* data;

		NodeInputPinData inputPins[10];
		NodeOutputPinData outputPins[9];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[5];

		/*!
		@brief The array of of list box data that are potentially displayed in
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

		ComputedParameterNodeData(const ComputedParameterNodeData& _cpnd) :
			NodeData(_cpnd), data{ _cpnd.data },
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

			inputPins[0].node = this;
			inputPins[1].node = this;
			inputPins[2].node = this;
			inputPins[3].node = this;
			inputPins[4].node = this;
			inputPins[5].node = this;
			inputPins[6].node = this;
			inputPins[7].node = this;
			inputPins[8].node = this;
			inputPins[9].node = this;

			outputPins[0].node = this;
			outputPins[1].node = this;
			outputPins[2].node = this;
			outputPins[3].node = this;
			outputPins[4].node = this;
			outputPins[5].node = this;
			outputPins[6].node = this;
			outputPins[7].node = this;
			outputPins[8].node = this;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		ComputedParameterNodeData(ECellEngine::Data::ComputedParameter* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			//ModelLinks Collapsing header
			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();

			//Asset
			inputPins[1] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);

			//Computed Parameters section
			inputPins[2] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[1] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();
			//lbsData[0] = { /*list of computed parameters where this one appears*/, GetMNBVCtxtNextId()}

			//Kinetic Laws section
			inputPins[3] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[2] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			//lbsData[1] = { /*list of kinetic laws where this one appears*/, GetMNBVCtxtNextId()}

			//Data Fields collapsing header
			inputPins[4] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[3] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();

			//Equation Operands collapsing header
			inputPins[5] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[4] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[4] = GetMNBVCtxtNextId();

			//Node String List Box for Species Operands
			inputPins[6] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[5] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);
			nslbData[2] = { &speciesOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Simple Parameter Operands
			inputPins[7] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[6] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);
			nslbData[3] = { &simpleParametersOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Computed Parameter Operands
			inputPins[8] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[7] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);
			nslbData[4] = { &computedParametersOperands, GetMNBVCtxtNextId() };

			//Value Float string of the computation of the formula
			//The user cannot directly change the value of the result
			inputPins[9] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
			outputPins[8] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
		}

		void InputConnect(std::size_t _nodeInputPinId) override {};//not used in computed parameter node data

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};//not used in computed parameter node data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override {};//not used in computed parameter node data

		void OutputConnect(std::size_t _nodeOutputPinId) override;

		void OutputUpdate(std::size_t _nodeOutputPinId) override;
	};

	struct LinePlotNodeData : public NodeData
	{
		char* name = "Line Plot";

		ScrollingBuffer dataPoints;
		float newPointBuffer[2] = { 0, 0 };
		unsigned short newPointConstructionCounter = 0;

		char plotTitle[64] = "PlotTitle";
		char xAxisLabel[64] = "x";
		char yAxisLabel[64] = "y";
		char lineLegend[64] = "f(x)";
		float plotSize[2] = { ImPlot::GetStyle().PlotDefaultSize.x, ImPlot::GetStyle().PlotDefaultSize.y };

		const ImGuiWindowFlags plotWindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
		ImPlotFlags plotFlags = ImPlotFlags_NoLegend | ImPlotFlags_NoInputs;
		ImPlotAxisFlags xAxisFlags = ImPlotAxisFlags_AutoFit;
		ImPlotAxisFlags yAxisFlags = ImPlotAxisFlags_AutoFit;

		NodeInputPinData inputPins[3];
		NodeOutputPinData outputPins[1];//not used

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[6];

		LinePlotNodeData(int _maxNbDataPoints, ImVec2& _position) :
			NodeData(), dataPoints{ _maxNbDataPoints }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);//Plot Collapsing Header
			inputPins[1] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);//X
			inputPins[2] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Y

			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this); //not used

			collapsingHeadersIds[0] = GetMNBVCtxtNextId();//Parameters Collapsing header
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();//General (Parameters) Collapsing header
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();//Plot Flags (Parameters) Collapsing header
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();//X Axis Flags (Parameters) Collapsing header
			collapsingHeadersIds[4] = GetMNBVCtxtNextId();//Y Axis Falgs (Parameters) Collapsing header
			collapsingHeadersIds[5] = GetMNBVCtxtNextId();//Plot Collapsing Header

			dataPoints.AddPoint(0, 0);
		}

		LinePlotNodeData(const LinePlotNodeData& _lpnd) :
			NodeData(_lpnd), dataPoints{ _lpnd.dataPoints },
			inputPins{ _lpnd.inputPins[0], _lpnd.inputPins[1] , _lpnd.inputPins[2] },
			outputPins{ _lpnd.outputPins[0] },
			utilityState{ _lpnd.utilityState },
			collapsingHeadersIds{ _lpnd.collapsingHeadersIds[0], _lpnd.collapsingHeadersIds[1],
			_lpnd.collapsingHeadersIds[2], _lpnd.collapsingHeadersIds[3], _lpnd.collapsingHeadersIds[4],
			_lpnd.collapsingHeadersIds[5] }
		{
			inputPins[0].node = this;
			inputPins[1].node = this;
			inputPins[2].node = this;

			outputPins[0].node = this;
		}

		void InputConnect(std::size_t _nodeInputPinId) override;

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};// not used in line plot node data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override;

		void OutputConnect(std::size_t _nodeOutputPinId) override {};// not used in line plot node data

		void OutputUpdate(std::size_t _nodeOutputPinId) override {};// not used in line plot node data

		inline bool IsPlotOpen() noexcept
		{
			return (utilityState >> 6) & 1;
		}

		inline void ResetNewPointBuffer()
		{
			newPointConstructionCounter = 0;
		}

		/*!
		@brief Utility function to switch the value 0 -> 1 or 1 -> 0 of the bit
				at position @p _stateBitPos in ::utilityState.
		@remarks It could be moved outside of this class in the future since the
				 code works with any data and is not pecific to this context.
		*/
		inline void SwitchState(const short _stateBitPos)
		{
			utilityState ^= (1 << _stateBitPos);
		}
	};

	struct ReactionNodeData : public NodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		//ax::NodeEditor::NodeId id;

		ECellEngine::Data::Reaction* data;

		NodeInputPinData inputPins[10];
		NodeOutputPinData outputPins[9];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[5];

		/*!
		@brief The array of of list box data that are potentially displayed in
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

		ReactionNodeData(const ReactionNodeData& _rnd) :
			NodeData(_rnd), data{ _rnd.data },
			inputPins{ _rnd.inputPins[0], _rnd.inputPins[1] , _rnd.inputPins[2] ,
					  _rnd.inputPins[3] , _rnd.inputPins[4] , _rnd.inputPins[5],
					  _rnd.inputPins[6] , _rnd.inputPins[7] , _rnd.inputPins[8], _rnd.inputPins[9] },
			outputPins{ _rnd.outputPins[0], _rnd.outputPins[1] , _rnd.outputPins[2] ,
					  _rnd.outputPins[3] , _rnd.outputPins[4] , _rnd.outputPins[5],
					  _rnd.outputPins[6] , _rnd.outputPins[7] , _rnd.outputPins[8] },
			utilityState{ _rnd.utilityState },
			collapsingHeadersIds{ _rnd.collapsingHeadersIds[0], _rnd.collapsingHeadersIds[1] , _rnd.collapsingHeadersIds[2] ,
					  _rnd.collapsingHeadersIds[3] , _rnd.collapsingHeadersIds[4] },
			nslbData{ _rnd.nslbData[0], _rnd.nslbData[1] , _rnd.nslbData[2] ,
					  _rnd.nslbData[3] , _rnd.nslbData[4] },
			speciesOperands{ _rnd.speciesOperands },
			simpleParametersOperands{ _rnd.simpleParametersOperands },
			computedParametersOperands{ _rnd.computedParametersOperands }
		{
			nslbData[2].data = &speciesOperands;
			nslbData[3].data = &simpleParametersOperands;
			nslbData[4].data = &computedParametersOperands;

			inputPins[0].node = this;
			inputPins[1].node = this;
			inputPins[2].node = this;
			inputPins[3].node = this;
			inputPins[4].node = this;
			inputPins[5].node = this;
			inputPins[6].node = this;
			inputPins[7].node = this;
			inputPins[8].node = this;

			outputPins[0].node = this;
			outputPins[1].node = this;
			outputPins[2].node = this;
			outputPins[4].node = this;
			outputPins[5].node = this;
			outputPins[6].node = this;
			outputPins[7].node = this;
			outputPins[8].node = this;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		ReactionNodeData(ECellEngine::Data::Reaction* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			//ModelLinks Collapsing header
			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();

			//Asset
			inputPins[1] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Reaction, this);

			//Reactants section
			inputPins[2] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[1] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();
			nslbData[0] = { _data->GetReactants() , GetMNBVCtxtNextId() };

			//Products section
			inputPins[3] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[2] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			nslbData[1] = { data->GetProducts(), GetMNBVCtxtNextId() };

			//Kinetic Law collapsing header
			inputPins[4] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[3] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();

			//Kinetic Law Operands collapsing header
			inputPins[5] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[4] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[4] = GetMNBVCtxtNextId();

			//Node String List Box for Species Operands, from Kinetic Law
			inputPins[6] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[5] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);
			nslbData[2] = { &speciesOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Simple Parameter Operands, from Kinetic Law
			inputPins[7] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[6] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);
			nslbData[3] = { &simpleParametersOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Computed Parameter Operands, from Kinetic Law
			inputPins[8] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[7] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);
			nslbData[4] = { &computedParametersOperands, GetMNBVCtxtNextId() };

			//Kinetic Law Value Float field (must be Read Only)
			inputPins[9] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
			outputPins[8] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
		}

		void InputConnect(std::size_t _nodeInputPinId) override {};//not used in Reaction Node Data

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};//not used in Reaction Node Data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override {};//not used in Reaction Node Data

		void OutputConnect(std::size_t _nodeOutputPinId) override;

		void OutputUpdate(std::size_t _nodeOutputPinId) override;
	};

	struct SimpleParameterNodeData : public NodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		//ax::NodeEditor::NodeId id;

		ECellEngine::Data::SimpleParameter* data;

		NodeInputPinData inputPins[6];
		NodeOutputPinData outputPins[5];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[4];

		NodeListBoxStringData nslbData[2];

		SimpleParameterNodeData(const SimpleParameterNodeData& _sprnd) :
			NodeData(_sprnd), data{ _sprnd.data },
			inputPins{ _sprnd.inputPins[0], _sprnd.inputPins[1] , _sprnd.inputPins[2] ,
					  _sprnd.inputPins[3] , _sprnd.inputPins[4] , _sprnd.inputPins[5] },
			outputPins{ _sprnd.outputPins[0], _sprnd.outputPins[1] , _sprnd.outputPins[2] ,
					  _sprnd.outputPins[3] , _sprnd.outputPins[4] },
			utilityState{ _sprnd.utilityState },
			collapsingHeadersIds{ _sprnd.collapsingHeadersIds[0], _sprnd.collapsingHeadersIds[1] , _sprnd.collapsingHeadersIds[2] ,
					  _sprnd.collapsingHeadersIds[3] },
			nslbData{ _sprnd.nslbData[0], _sprnd.nslbData[1] }
		{
			inputPins[0].node = this;
			inputPins[1].node = this;
			inputPins[2].node = this;
			inputPins[3].node = this;
			inputPins[4].node = this;
			inputPins[5].node = this;

			outputPins[0].node = this;
			outputPins[1].node = this;
			outputPins[2].node = this;
			outputPins[4].node = this;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		SimpleParameterNodeData(ECellEngine::Data::SimpleParameter* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			//ModelLinks Collapsing header
			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();

			//Asset
			inputPins[1] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);

			//Computed Parameters section
			inputPins[2] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[1] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();
			//lbsData[0] = { /*list of computed parameters where this one appears*/, GetMNBVCtxtNextId()}

			//Kinetic Laws section
			inputPins[3] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[2] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			//lbsData[1] = { /*list of kinetic laws where this one appears*/, GetMNBVCtxtNextId()}

			//Data Fields collapsing header
			inputPins[4] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[3] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			collapsingHeadersIds[3] = GetMNBVCtxtNextId();

			//Value Float field
			inputPins[5] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
			outputPins[4] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
		}

		void InputConnect(std::size_t _nodeInputPinId) override;

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};//not used in Reaction Node Data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override;

		void OutputConnect(std::size_t _nodeOutputPinId) override;

		void OutputUpdate(std::size_t _nodeOutputPinId) override;
	};

	struct SimulationTimeNodeData : public NodeData
	{
		ECellEngine::Core::Timer* simulationTimer;
		float elapsedTimeBuffer = 0.f;

		NodeInputPinData inputPins[1];
		NodeOutputPinData outputPins[1];

		SimulationTimeNodeData(ECellEngine::Core::Timer* _simulationTimer, ImVec2& _position) :
			NodeData(), simulationTimer{ _simulationTimer }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this); //not used
			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this); //simulation Time
		}

		SimulationTimeNodeData(const SimulationTimeNodeData& _stnd) :
			NodeData(_stnd), simulationTimer{ _stnd.simulationTimer },
			inputPins{ _stnd.inputPins[0] },
			outputPins{ _stnd.outputPins[0] }
		{
			inputPins[0].node = this;
			outputPins[0].node = this;
		}

		void InputConnect(std::size_t _nodeInputPinId) override {};//not used in Simulation Time Node Data

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};//not used in Simulation Time Node Data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override {};//not used in Simulation Time Node Data

		void OutputConnect(std::size_t _nodeOutputPinId) override {};//not used in Simulation Time Node Data

		void OutputUpdate(std::size_t _nodeOutputPinId) override;
	};

	struct SolverNodeData : public NodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		//ax::NodeEditor::NodeId id;

		ECellEngine::Solvers::Solver* data;

		NodeInputPinData inputPins[1];
		NodeOutputPinData outputPins[1];

		SolverNodeData(const SolverNodeData& _slvnd) :
			NodeData(_slvnd), data{ _slvnd.data },
			inputPins{ _slvnd.inputPins[0] },
			outputPins{ _slvnd.outputPins[0] }
		{
			inputPins[0].node = this;
			outputPins[0].node = this;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		SolverNodeData(ECellEngine::Solvers::Solver* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			//Not used for now but added for homogeneity
			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);

			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Solver, this);

		}

		void InputConnect(std::size_t _nodeInputPinId) override {};//not used in Solver Node Data

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};//not used in Solver Node Data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override {};//not used in Solver Node Data

		void OutputConnect(std::size_t _nodeOutputPinId) override {};//not used in Solver Node Data

		void OutputUpdate(std::size_t _nodeOutputPinId) override;
	};

	struct SpeciesNodeData : public NodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		//ax::NodeEditor::NodeId id;

		ECellEngine::Data::Species* data;
		float speciesQuantityBuffer = 0.f;

		NodeInputPinData inputPins[8];
		NodeOutputPinData outputPins[7];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[2];

		SpeciesNodeData(const SpeciesNodeData& _snd) :
			NodeData(_snd), data{ _snd.data }, speciesQuantityBuffer{ _snd.speciesQuantityBuffer },
			inputPins{ _snd.inputPins[0], _snd.inputPins[1] , _snd.inputPins[2] ,
					  _snd.inputPins[3] , _snd.inputPins[4] , _snd.inputPins[5],
					  _snd.inputPins[6] , _snd.inputPins[7] },
			outputPins{ _snd.outputPins[0], _snd.outputPins[1] , _snd.outputPins[2] ,
					  _snd.outputPins[3] , _snd.outputPins[4] , _snd.outputPins[5],
					  _snd.outputPins[6] },
			utilityState{ _snd.utilityState },
			collapsingHeadersIds{ _snd.collapsingHeadersIds[0], _snd.collapsingHeadersIds[1] }
		{
			inputPins[0].node = this;
			inputPins[1].node = this;
			inputPins[2].node = this;
			inputPins[3].node = this;
			inputPins[4].node = this;
			inputPins[5].node = this;
			inputPins[6].node = this;
			inputPins[7].node = this;

			outputPins[0].node = this;
			outputPins[1].node = this;
			outputPins[2].node = this;
			outputPins[4].node = this;
			outputPins[5].node = this;
			outputPins[6].node = this;
		}

		SpeciesNodeData(ECellEngine::Data::Species* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			//Id to uniquely identify the collapsing header for the MODEL LINKS of each species node
			collapsingHeadersIds[0] = GetMNBVCtxtNextId();
			//Id to uniquely identify the collapsing header for the DATA FIELDS of each species node
			collapsingHeadersIds[1] = GetMNBVCtxtNextId();

			//Collapsing Header Model Links
			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);

			//Asset
			inputPins[1] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);

			//Computed parameters equation
			inputPins[2] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[1] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);

			//Reactions's Reactants
			inputPins[3] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[2] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);

			//Reaction's Products
			inputPins[4] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[3] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);

			//Reaction's Kinetic Law
			inputPins[5] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[4] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);

			//Collapsing Header Data Fields
			inputPins[6] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);
			outputPins[5] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);

			//Quantity
			inputPins[7] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
			outputPins[6] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
		}

		void InputConnect(std::size_t _nodeInputPinId) override;

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};//not used in Species Node Data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override;

		void OutputConnect(std::size_t _nodeOutputPinId) override;

		void OutputUpdate(std::size_t _nodeOutputPinId) override;
	};

	struct ValueFloatNodeData : public NodeData
	{
		float value = 0.f;

		NodeInputPinData inputPins[1];
		NodeOutputPinData outputPins[1];

		ValueFloatNodeData(float _value, ImVec2& _position) :
			NodeData(), value{ _value }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[0] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this); //not used
			outputPins[0] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this); //simulation Time
		}

		ValueFloatNodeData(const ValueFloatNodeData& _vfnd) :
			NodeData(_vfnd), value{ _vfnd.value },
			inputPins{ _vfnd.inputPins[0] },
			outputPins{ _vfnd.outputPins[0] }
		{
			inputPins[0].node = this;
			outputPins[0].node = this;
		}

		void InputConnect(std::size_t _nodeInputPinId) override {};//not used in Value Float Node Data

		void InputUpdate(std::size_t _nodeInputPinId, char* _data) override {};//not used in Value Float Node Data

		void InputUpdate(std::size_t _nodeInputPinId, float _data) override {};//not used in Value Float Node Data

		void OutputConnect(std::size_t _nodeOutputPinId) override {};//not used in Value Float Node Data

		void OutputUpdate(std::size_t _nodeOutputPinId) override;
	};
#pragma endregion
}