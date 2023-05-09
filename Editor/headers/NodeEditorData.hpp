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

		friend inline bool operator==(const LinkData& lhs, const LinkData& rhs) { return lhs.id.Get() == rhs.id.Get(); }
		friend inline bool operator!=(const LinkData& lhs, const LinkData& rhs) { return !(lhs == rhs); }

		friend inline bool operator< (const LinkData& lhs, const LinkData& rhs) { return lhs.id.Get() < rhs.id.Get(); }
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

	struct NodeInputPinData;
	struct NodeOutputPinData;

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

		friend inline bool operator==(const NodeData& lhs, const NodeData& rhs) { return lhs.id.Get() == rhs.id.Get(); }
		friend inline bool operator!=(const NodeData& lhs, const NodeData& rhs) { return !(lhs == rhs); }

		friend inline bool operator< (const NodeData& lhs, const NodeData& rhs) { return lhs.id.Get() < rhs.id.Get(); }
		friend inline bool operator> (const NodeData& lhs, const NodeData& rhs) { return rhs < lhs; }
		friend inline bool operator<=(const NodeData& lhs, const NodeData& rhs) { return !(lhs > rhs); }
		friend inline bool operator>=(const NodeData& lhs, const NodeData& rhs) { return !(lhs < rhs); }

		virtual void InputConnect(const NodeInputPinData& _nodeInput) = 0;

		virtual void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) = 0;

		virtual void InputUpdate(const NodeInputPinData& _nodeInput, float _data) = 0;

		virtual void OutputConnect(const NodeOutputPinData& _nodeOutput) = 0;

		virtual void OutputUpdate(const NodeOutputPinData& _nodeOutput) = 0;
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
			id{ _pinId }, kind{ _kind }, type{ _type }, node{ _node }
		{

		}

		inline operator std::size_t() { return id.Get(); }

		friend inline bool operator==(const NodePinData& lhs, const NodePinData& rhs) { return lhs.id.Get() == rhs.id.Get(); }
		friend inline bool operator!=(const NodePinData& lhs, const NodePinData& rhs) { return !(lhs == rhs); }

		friend inline bool operator< (const NodePinData& lhs, const NodePinData& rhs) { return lhs.id.Get() < rhs.id.Get(); }
		friend inline bool operator> (const NodePinData& lhs, const NodePinData& rhs) { return rhs < lhs; }
		friend inline bool operator<=(const NodePinData& lhs, const NodePinData& rhs) { return !(lhs > rhs); }
		friend inline bool operator>=(const NodePinData& lhs, const NodePinData& rhs) { return !(lhs < rhs); }
	};
#pragma endregion

#pragma region Derived Pin Data
	struct NodeInputPinData : public NodePinData
	{
		NodeInputPinData() = default;

		NodeInputPinData(std::size_t& _pinId, PinType _type, NodeData* _node) :
			NodePinData(_pinId, ax::NodeEditor::PinKind::Input, _type, _node)
		{

		}

		template<class Data>
		inline void Receive(Data _data)
		{
			node->InputUpdate(*this, _data);
		}
	};

	struct NodeOutputPinData : public NodePinData
	{
		std::vector<NodeInputPinData*> subscribers;

		NodeOutputPinData() = default;

		NodeOutputPinData(std::size_t& _pinId, PinType _type, NodeData* _node) :
			NodePinData(_pinId, ax::NodeEditor::PinKind::Output, _type, _node)
		{

		}

		inline void AddSubscriber(NodeInputPinData* _newSubscriber)
		{
			subscribers.push_back(_newSubscriber);

			//What to do on connection.
			node->OutputConnect(*this);
			_newSubscriber->node->InputConnect(*_newSubscriber);

			//Send data for the first time
			node->OutputUpdate(*this);
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
		enum InputPin
		{
			InputPin_Solver,

			InputPin_Count
		};

		enum OutputPin
		{
			OutputPin_CollHdrComputedParameters,
			OutputPin_CollHdrReactions,
			OutputPin_CollHdrSimpleParameters,
			OutputPin_CollHdrSpecies,

			OutputPin_Count
		};

		enum CollapsingHeader
		{
			CollapsingHeader_ComputedParameters,
			CollapsingHeader_Reactions,
			CollapsingHeader_SimpleParameters,
			CollapsingHeader_Species,

			CollapsingHeader_Count
		};

		enum NodeListBoxString
		{
			NodeListBoxString_ComputedParameters,
			NodeListBoxString_Reactions,
			NodeListBoxString_SimpleParameters,
			NodeListBoxString_Species,

			NodeListBoxString_Count
		};

		enum State
		{
			State_CollHdrComputedParameters,
			State_CollHdrReactions,
			State_CollHdrSimpleParameters,
			State_CollHdrSpecies,

			State_Count
		};
		
		ECellEngine::Data::SBMLModule* data;

		NodeInputPinData inputPins[InputPin_Count];
		NodeOutputPinData outputPins[OutputPin_Count];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[4];

		NodeListBoxStringData nlbsData[NodeListBoxString_Count];

		AssetNodeData(const AssetNodeData& _and) :
			NodeData(_and), data{ _and.data },
			inputPins{ _and.inputPins[0] },
			outputPins{ _and.outputPins[0], _and.outputPins[1] , _and.outputPins[2] ,
					  _and.outputPins[3] },
			utilityState{ _and.utilityState },
			collapsingHeadersIds{ _and.collapsingHeadersIds[0], _and.collapsingHeadersIds[1] , _and.collapsingHeadersIds[2] ,
					  _and.collapsingHeadersIds[3] },
			nlbsData{ _and.nlbsData[0], _and.nlbsData[1] , _and.nlbsData[2] , _and.nlbsData[3] }
		{
			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		AssetNodeData(ECellEngine::Data::Module* _data) :
			NodeData(), data{ dynamic_cast<ECellEngine::Data::SBMLModule*>(_data) }
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			inputPins[InputPin_Solver] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Solver, this);//Solver

			outputPins[OutputPin_CollHdrComputedParameters] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);//Species Collapsing header
			outputPins[OutputPin_CollHdrReactions] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Simple Parameters Collapsing header
			outputPins[OutputPin_CollHdrSimpleParameters] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters Collapsing header
			outputPins[OutputPin_CollHdrSpecies] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Reaction, this);//Reactions Collapsing header

			collapsingHeadersIds[CollapsingHeader_ComputedParameters] = GetMNBVCtxtNextId();//Species Collapsing header
			collapsingHeadersIds[CollapsingHeader_Reactions] = GetMNBVCtxtNextId();//Simple Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_SimpleParameters] = GetMNBVCtxtNextId();//Computed Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_Species] = GetMNBVCtxtNextId();//Reactions Collapsing header

			//Initialize the list boxes data
			nlbsData[NodeListBoxString_ComputedParameters] = { &data->GetAllComputedParameter(), GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_Reactions] = { &data->GetAllReaction(), GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_SimpleParameters] = { &data->GetAllSimpleParameter(), GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_Species] = { &data->GetAllSpecies(), GetMNBVCtxtNextId() };

		}

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in asset node data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override;

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in asset node data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in asset node data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override {};//not used in asset node data
	};

	struct ComputedParameterNodeData : public NodeData
	{
		enum InputPin
		{
			InputPin_CollHdrModelLinks,
			InputPin_CollHdrComputedParameters,
			InputPin_CollHdrKineticLaws,
			InputPin_CollHdrDataFields,
			InputPin_CollHdrEquationOperands,
			InputPin_NLBSComputedParameters,
			InputPin_NLBSSimpleParameters,
			InputPin_NLBSSpecies,
			InputPin_ComputedParameterValue,
			InputPin_Asset,

			InputPin_Count
		};

		enum OutputPin
		{
			OutputPin_CollHdrModelLinks,
			OutputPin_CollHdrComputedParameters,
			OutputPin_CollHdrKineticLaws,
			OutputPin_CollHdrDataFields,
			OutputPin_CollHdrEquationOperands,
			OutputPin_NLBSComputedParameters,
			OutputPin_NLBSSimpleParameters,
			OutputPin_NLBSSpecies,
			OutputPin_ComputedParameterValue,

			OutputPin_Count
		};

		enum CollapsingHeader
		{
			CollapsingHeader_ModelLinks,
			CollapsingHeader_ComputedParameters,
			CollapsingHeader_KineticLaws,
			CollapsingHeader_DataFields,
			CollapsingHeader_EquationOperands,

			CollapsingHeader_Count
		};

		enum NodeListBoxString
		{
			NodeListBoxString_ComputedParameterLinks,
			NodeListBoxString_ReactionLinks,
			NodeListBoxString_ComputedParameterOperands,
			NodeListBoxString_SimpleParameterOperands,
			NodeListBoxString_SpeciesOperands,

			NodeListBoxString_Count
		};

		enum State
		{
			State_CollHdrModelLinks,
			State_CollHdrComputedParameters,
			State_CollHdrKineticLaws,
			State_CollHdrDataFields,
			State_CollHdrEquationOperands,

			State_Count
		};

		ECellEngine::Data::ComputedParameter* data;

		NodeInputPinData inputPins[InputPin_Count];
		NodeOutputPinData outputPins[OutputPin_Count];

		unsigned char utilityState = 0;

		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

		/*!
		@brief The array of of list box data that are potentially displayed in
				this node.
		*/
		NodeListBoxStringData nlbsData[NodeListBoxString_Count];
		std::vector<std::string> computedParameterLinks;
		std::vector<std::string> reactionLinks;
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
			nlbsData{ _cpnd.nlbsData[0], _cpnd.nlbsData[1] , _cpnd.nlbsData[2] ,
					  _cpnd.nlbsData[3] , _cpnd.nlbsData[4] },
			speciesOperands{ _cpnd.speciesOperands },
			simpleParametersOperands{ _cpnd.simpleParametersOperands },
			computedParametersOperands{ _cpnd.computedParametersOperands }
		{
			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}

			nlbsData[NodeListBoxString_ComputedParameterLinks].data = &computedParameterLinks;
			nlbsData[NodeListBoxString_ReactionLinks].data = &reactionLinks;
			nlbsData[NodeListBoxString_SpeciesOperands].data = &speciesOperands;
			nlbsData[NodeListBoxString_SimpleParameterOperands].data = &simpleParametersOperands;
			nlbsData[NodeListBoxString_ComputedParameterOperands].data = &computedParametersOperands;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		ComputedParameterNodeData(ECellEngine::Data::ComputedParameter* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));
			
			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			inputPins[InputPin_Asset] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Asset, this);//Asset
			inputPins[InputPin_CollHdrComputedParameters] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			inputPins[InputPin_CollHdrKineticLaws] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			inputPins[InputPin_CollHdrDataFields] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			inputPins[InputPin_CollHdrEquationOperands] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Default, this);//Equation Operands collapsing header
			inputPins[InputPin_NLBSSpecies] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);//Node String List Box for Species Operands
			inputPins[InputPin_NLBSSimpleParameters] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Simple Parameter Operands
			inputPins[InputPin_NLBSComputedParameters] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Computed Parameter Operands
			inputPins[InputPin_ComputedParameterValue] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Operation Value Float
			
			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			outputPins[OutputPin_CollHdrComputedParameters] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			outputPins[OutputPin_CollHdrKineticLaws] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			outputPins[OutputPin_CollHdrDataFields] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			outputPins[OutputPin_CollHdrEquationOperands] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Default, this);//Equation Operands collapsing header
			outputPins[OutputPin_NLBSSpecies] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);//Node String List Box for Species Operands
			outputPins[OutputPin_NLBSSimpleParameters] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Simple Parameter Operands
			outputPins[OutputPin_NLBSComputedParameters] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Computed Parameter Operands
			outputPins[OutputPin_ComputedParameterValue] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Operation Value Float

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = GetMNBVCtxtNextId();//ModelLinks Collapsing header
			collapsingHeadersIds[CollapsingHeader_ComputedParameters] = GetMNBVCtxtNextId();//Computed Parameters section
			collapsingHeadersIds[CollapsingHeader_KineticLaws] = GetMNBVCtxtNextId();//Kinetic Laws section
			collapsingHeadersIds[CollapsingHeader_DataFields] = GetMNBVCtxtNextId();//Data Fields collapsing header
			collapsingHeadersIds[CollapsingHeader_EquationOperands] = GetMNBVCtxtNextId();//Equation Operands collapsing header

			nlbsData[NodeListBoxString_ComputedParameterLinks] = { &computedParameterLinks , GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_ReactionLinks] = { &reactionLinks, GetMNBVCtxtNextId() };
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);//Node String List Box for Species Operands
			nlbsData[NodeListBoxString_SpeciesOperands] = { &speciesOperands, GetMNBVCtxtNextId() };//Node String List Box for Species Operands
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);//Node String List Box for Simple Parameter Operands
			nlbsData[NodeListBoxString_SimpleParameterOperands] = { &simpleParametersOperands, GetMNBVCtxtNextId() };//Node String List Box for Simple Parameter Operands
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);//Node String List Box for Computed Parameter Operands
			nlbsData[NodeListBoxString_ComputedParameterOperands] = { &computedParametersOperands, GetMNBVCtxtNextId() };//Node String List Box for Computed Parameter Operands
		}

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in computed parameter node data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in computed parameter node data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in computed parameter node data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
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

		void InputConnect(const NodeInputPinData& _nodeInput) override;

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};// not used in line plot node data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override;

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};// not used in line plot node data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override {};// not used in line plot node data

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
		NodeListBoxStringData nlbsData[5];
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
			nlbsData{ _rnd.nlbsData[0], _rnd.nlbsData[1] , _rnd.nlbsData[2] ,
					  _rnd.nlbsData[3] , _rnd.nlbsData[4] },
			speciesOperands{ _rnd.speciesOperands },
			simpleParametersOperands{ _rnd.simpleParametersOperands },
			computedParametersOperands{ _rnd.computedParametersOperands }
		{
			nlbsData[2].data = &speciesOperands;
			nlbsData[3].data = &simpleParametersOperands;
			nlbsData[4].data = &computedParametersOperands;

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
			nlbsData[0] = { _data->GetReactants() , GetMNBVCtxtNextId() };

			//Products section
			inputPins[3] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			outputPins[2] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Species, this);
			collapsingHeadersIds[2] = GetMNBVCtxtNextId();
			nlbsData[1] = { data->GetProducts(), GetMNBVCtxtNextId() };

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
			nlbsData[2] = { &speciesOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Simple Parameter Operands, from Kinetic Law
			inputPins[7] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[6] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);
			nlbsData[3] = { &simpleParametersOperands, GetMNBVCtxtNextId() };

			//Node String List Box for Computed Parameter Operands, from Kinetic Law
			inputPins[8] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			outputPins[7] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_Parameter, this);
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);
			nlbsData[4] = { &computedParametersOperands, GetMNBVCtxtNextId() };

			//Kinetic Law Value Float field (must be Read Only)
			inputPins[9] = NodeInputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
			outputPins[8] = NodeOutputPinData(GetMNBVCtxtNextId(), PinType_ValueFloat, this);
		}

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Reaction Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Reaction Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Reaction Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
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

		NodeListBoxStringData nlbsData[2];

		SimpleParameterNodeData(const SimpleParameterNodeData& _sprnd) :
			NodeData(_sprnd), data{ _sprnd.data },
			inputPins{ _sprnd.inputPins[0], _sprnd.inputPins[1] , _sprnd.inputPins[2] ,
					  _sprnd.inputPins[3] , _sprnd.inputPins[4] , _sprnd.inputPins[5] },
			outputPins{ _sprnd.outputPins[0], _sprnd.outputPins[1] , _sprnd.outputPins[2] ,
					  _sprnd.outputPins[3] , _sprnd.outputPins[4] },
			utilityState{ _sprnd.utilityState },
			collapsingHeadersIds{ _sprnd.collapsingHeadersIds[0], _sprnd.collapsingHeadersIds[1] , _sprnd.collapsingHeadersIds[2] ,
					  _sprnd.collapsingHeadersIds[3] },
			nlbsData{ _sprnd.nlbsData[0], _sprnd.nlbsData[1] }
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

		void InputConnect(const NodeInputPinData& _nodeInput) override;

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Reaction Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override;

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
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

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Simulation Time Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Simulation Time Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Simulation Time Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in Simulation Time Node Data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
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

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Solver Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Solver Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Solver Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in Solver Node Data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
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

		void InputConnect(const NodeInputPinData& _nodeInput) override;

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Species Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override;

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
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

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Value Float Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Value Float Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Value Float Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in Value Float Node Data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
	};
#pragma endregion
}