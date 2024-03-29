#pragma once
/*!
@file
@brief The source of the data structures describing nodes, pins and links
		that are used to know how to draw these entities in ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget
*/

#include "implot.h"
#include "imgui_node_editor.h"

#include "Core/Callback.hpp"
#include "Data/BinaryOperatedVector.hpp"
#include "Core/Timer.hpp"
#include "Utility/Plot/LinePlot.hpp"
#include "Widget/MNBV/ModelNodeBasedViewerGlobal.hpp"

namespace ECellEngine::Editor::Utility::MNBV
{
#pragma region Custom Node Widgets Data
	/*!
	@brief Information to display and interact with custom list boxes inside
			a node.
	@see It is used in ECellEngine::Editor::Utility::NodeEditorDraw::NodeStringListBox
	*/
	template<typename DataType>
	struct NodeListBoxStringData
	{
		const std::vector<DataType>* data = nullptr;

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

		NodeListBoxStringData(const std::vector<DataType>* _data,  std::size_t _scrollBarId) :
			data{_data}, cursor{ _data->size()}, scrollBarID{_scrollBarId}
		{

		}

		const char* At(std::size_t _idx) const noexcept;

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

		const DataType& GetDoubleClickedItem() const noexcept
		{
			return data->at(doubleClickedItem);
		}

		const DataType& GetHoveredItem() const noexcept
		{
			return data->at(hoveredItem);
		}

		const DataType& GetSelectedItem() const noexcept
		{
			return data->at(selectedItem);
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

#pragma region Base Node, Pin
	struct NodeInputPinData;
	struct NodeOutputPinData;

	/*!
	@brief The base class of for nodes.
	*/
	struct NodeData
	{
		/*!
		@brief The ID of this node to in the Node Editor.
		*/
		ax::NodeEditor::NodeId id;

		NodeData() :
			id{ Widget::MNBV::GetMNBVCtxtNextId() }
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

		/*!
		@brief Method to implement what to do when a pin (@p _nodeInput) receives
				a link connection with some data.
		@param _nodeInput The pin notifying this node that a data has arrived
				through it because of a new link connection.
		@param _nodeOuput The pin from which the data has arrived.
		@param _data The data received through the link.
		*/
		virtual void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) = 0;

		/*!
		@brief Method to implement what to do when a link is disconnected
				between @p _nodeInput and @p _nodeOutput.
		@param _nodeInput The pin notifying this node that a link has been
				disconnected.
		@param _nodeOuput The pin from which the data has arrived.
		@param _data Data that might be usefull to help correctly disconnect.
		*/
		virtual void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) = 0;

		/*!
		@brief Method to refresh data links in the end node.
		@details Typically used to correct invalidated pointers in the end
				 node.
		@param _nodeInput The pin notifying this node that the data transiting
				through a link must be refreshed.
		@param _nodeOuput The pin from which the data originates (or the node it
				represents).
		*/
		virtual void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) = 0;

		/*!
		@brief Method to implement what to do when a new link is connected
				from @p _nodeOutput to @p _nodeInput.
		@param _nodeInput The pointer to the end pin of the link.
		@param _nodeOutput The pin notifying this node that a link was made with
				it as the start.
		*/
		virtual void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) = 0;

		/*!
		@brief Method to implement what to do when a link is disconnected
				between @p _nodeOutput and @p _nodeInput.
		@param _nodeInput The pointer to the end pin of the link.
		@param _nodeOuput The pin notifying this node that a link has been
				disconnected.
		*/
		virtual void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) = 0;

		/*!
		@brief Method to refresh data links in the start node.
		@details Typically used to correct invalidated pointers in the start
				 node.
		@param _nodeInput The pin where which the data goes (or the node it
				represents).
		@param _nodeOuput The pin notifying this node that the data transiting
				through a link must be refreshed..
		*/
		virtual void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) = 0;
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

		/*!
		@brief Whether the pin is Input (receives data) or Output (broadcast
				data).
		*/
		ax::NodeEditor::PinKind kind = ax::NodeEditor::PinKind::Input;

		/*!
		@brief The type of the pin.
		@details used to control which pin can be connected to which.
		*/
		PinType type = PinType_Default;

		/*!
		@brief The pointer to the node this Pin is in.
		*/
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

	/*!
	@brief The logic for input pins.
	@details Implements the "Subscriber" part of a Publish-Subscribe design
			 pattern. (aka Observer)
	@see ECellEngine::Editor::Utility::NodeOutputPinData for the "Publisher"
		 part.
	*/
	struct NodeInputPinData final : public NodePinData
	{
		NodeInputPinData() = default;

		NodeInputPinData(std::size_t& _pinId, PinType _type, NodeData* _node) :
			NodePinData(_pinId, ax::NodeEditor::PinKind::Input, _type, _node)
		{

		}

		/*!
		@brief What to do when a link gets connected to the input pin.
		@param _data The pointer the the data this pin receives.
		*/
		inline void OnConnect(NodeOutputPinData* _outputNode, void* _data)
		{
			node->InputConnect(this, _outputNode, _data);

			isUsed = true;
		}

		/*!
		@brief What to do when a link gets disconnected from the input pin.
		@param _nodeOutput The output pin data which was connected to this input
				pin data.
		@param _data Data that might be useful to disconnect the link
		*/
		inline void OnDisconnect(NodeOutputPinData* _nodeOutput, void* _data)
		{
			node->InputDisconnect(this, _nodeOutput, _data);

			isUsed = false;
		}

		/*!
		@brief To notify the node that the pointer ::_data has been invalidated
				and needs to be refreshed.
		@param _nodeOutput The output pin data which is connected to this input
				pin data.
		*/
		inline void OnRefresh(NodeOutputPinData* _outputNode, void* _data)
		{
			node->InputRefresh(this, _outputNode, _data);
		}
	};

	/*!
	@brief The logic for ouput pins.
	@details Implements the "Publisher" part of a Publish-Subscribe design
			 pattern. (aka Observer)
	@see ECellEngine::Editor::Utility::NodeInputPinData for the "Subscriber"
		 part.
	*/
	struct NodeOutputPinData final : public NodePinData
	{
		NodeOutputPinData() = default;

		NodeOutputPinData(std::size_t& _pinId, PinType _type, NodeData* _node) :
			NodePinData(_pinId, ax::NodeEditor::PinKind::Output, _type, _node)
		{

		}

		/*!
		@brief What happens when a link is created between this pin and
				@p _nodeInput.
		@details Typically happens every time a dynamic link is created.
		@param _inputNode The input pin data which we connect to this output
				pin data.
		*/
		inline void OnConnect(NodeInputPinData* _inputNode)
		{
			//What to do on connection.
			node->OutputConnect(_inputNode, this);

			isUsed = true;
		}

		/*!
		@brief What to do when a link gets disconnected from this output pin.
		@param _inputNode The input pin data which was connected to this output
				pin data.
		*/
		inline void OnDisconnect(NodeInputPinData* _inputNode)
		{
			//What to do on disconnection.
			node->OutputDisconnect(_inputNode, this);

			isUsed = false;
		}

		/*!
		@brief To notify the node that the data exchange with @p _inputNode has
				been invalidated and needs to be refreshed.
		@param _inputNode The input pin data which is connected to this output
				pin data.
		*/
		inline void OnRefresh(NodeInputPinData* _inputNode)
		{
			node->OutputRefresh(_inputNode, this);
		}
	};

	/*!
	@brief Information about connection between pins.
	@details Note that connection (aka. link) has its own ID. This is useful
			 later with dealing with selections, deletion or other operations.
	@remarks Originally from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct LinkData
	{
		/*!
		@brief The id of this link
		*/
		ax::NodeEditor::LinkId id;

		/*!
		@brief Pointer to the pin data used as the start of this link.
		*/
		NodeOutputPinData* startPin;

		/*!
		@brief Pointer to the pin data used as the end of this link.
		*/
		NodeInputPinData* endPin;

		/*!
		@brief The ids of the pins to use as the start of the link.
		@details At index 0 is the main pin while index 1 contains the if of the
				 fallback pin.
		*/
		ax::NodeEditor::PinId startIds[2];

		/*!
		@brief The ids of the pins to use use as the end of the link.
		@details At index 0 is the main pin while index 1 contains the if of the
				 fallback pin.
		*/
		ax::NodeEditor::PinId endIds[2];

		LinkData(NodePinData* _startPin, NodePinData* _endPin) :
			id{ Widget::MNBV::GetMNBVCtxtNextId() },
			startPin{ (NodeOutputPinData*)_startPin },
			endPin{ (NodeInputPinData*)_endPin },
			startIds{ _startPin->id, _startPin->id }, endIds{ _endPin->id, _endPin->id }
		{

		}
		LinkData(const LinkData& _other) :
			id{ _other.id }, startPin{ _other.startPin }, endPin{ _other.endPin },
			startIds{ _other.startIds[0], _other.startIds[1] },
			endIds{ _other.endIds[0], _other.endIds[1] }
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

		void Refresh();
	};

#pragma endregion

#pragma region Derived Nodes Data

	/*!
	@brief The logic to encode the data needed to draw the node to define and
			use a logic operation.
	*/
	struct ArithmeticOperationNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_LHS,
			InputPin_RHS,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_OnOperandChange,
			OutputPin_OnResultChange,

			OutputPin_Count
		};

		/*!
		@brief The display list of the available arithmetic operators.
		*/
		const char* operatorTypes[Maths::FunctionType_Count] = { 
			"IDENTITY", "PLUS", "MINUS", "TIMES", "DIVIDE",
			"POWER",	"ROOT"};

		/*!
		@brief The default operand to hold a value in this watcher when nothing
				is connected in the input pin of the LHS.
		@details We use the class of a parameter as the default operand because
				 it is the less intrusive type of operand which value can be
				 changed at runtime.
		*/
		Data::Parameter lhs;

		/*!
		@brief The default operand to hold a value in this watcher when nothing
				is connected in the input pin of the RHS.
		@details We use the class of a parameter as the default operand because
				 it is the less intrusive type of operand which value can be
				 changed at runtime.
		*/
		Data::Parameter rhs;

		/*!
		@brief Pointer to the arithemtic operation represented by this node.
		*/
		std::shared_ptr<Maths::Operation> data;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		ArithmeticOperationNodeData(std::shared_ptr<Maths::Operation> _data, ImVec2& _position) :
			NodeData(), data{ _data },
			lhs{ "Operation[" + std::to_string((std::size_t)id) + "]::LHS", 0 },
			rhs{ "Operation[" + std::to_string((std::size_t)id) + "]::RHS", 0 }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			data->name = "Operation[" + std::to_string((std::size_t)id) + "]";
			data->AddOperand(&lhs);
			data->AddOperand(&rhs);

			inputPins[InputPin_LHS] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackSubscriber, this); //Left Hand Side
			inputPins[InputPin_RHS] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackSubscriber, this); //Right Hand Side
			outputPins[OutputPin_OnOperandChange] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackPublisher, this); //The callback when an operand (input) changes
			outputPins[OutputPin_OnResultChange] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackPublisher, this); //The callback when the result changes
		}

		ArithmeticOperationNodeData(const ArithmeticOperationNodeData& _aond) :
			NodeData(_aond),
			lhs{ _aond.lhs }, rhs{ _aond.rhs }, data{ _aond.data },
			inputPins{ _aond.inputPins[0], _aond.inputPins[1] },
			outputPins{ _aond.outputPins[0], _aond.outputPins[1] }
		{
			data->OverrideOperand(&lhs, 0);
			data->OverrideOperand(&rhs, 1);

			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}
		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Logic Operation Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in Logic Operation Node Data
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::SBMLModule.
	*/
	struct AssetNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_Solver,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_CollHdrEquations,
			OutputPin_CollHdrReactions,
			OutputPin_CollHdrParameters,
			OutputPin_CollHdrSpecies,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_Equations,
			CollapsingHeader_Reactions,
			CollapsingHeader_Parameters,
			CollapsingHeader_Species,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage access to the list boxes storing strings.
		@see ::nlbsData
		*/
		enum NodeListBoxString
		{
			NodeListBoxString_Equations,
			NodeListBoxString_Reactions,
			NodeListBoxString_Parameters,
			NodeListBoxString_Species,

			NodeListBoxString_Count
		};

		/*!
		@brief The local enum to manage to the encoding of the state of this
				node.
		@see ::utilityState
		*/
		enum State
		{
			State_CollHdrEquations,
			State_CollHdrReactions,
			State_CollHdrParameters,
			State_CollHdrSpecies,

			State_Count
		};

		/*!
		@brief Pointer to the sbml module represented by this node.
		*/
		ECellEngine::Data::BiochemicalModule* data;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		/*!
		@brief The byte the encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned short utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[4];

		/*!
		@brief All the list boxes to store/display strings.
		@details Access the pins with the enum values NodeListBoxString_XXX
		*/
		NodeListBoxStringData<std::string> nlbsData[NodeListBoxString_Count];

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

		AssetNodeData(ECellEngine::Data::Module* _data) :
			NodeData(), data{ dynamic_cast<ECellEngine::Data::BiochemicalModule*>(_data) }
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			inputPins[InputPin_Solver] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Solver, this);//Solver

			outputPins[OutputPin_CollHdrEquations] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Asset, this);//Species Collapsing header
			outputPins[OutputPin_CollHdrReactions] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Asset, this);//Simple Parameters Collapsing header
			outputPins[OutputPin_CollHdrParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Asset, this);//Computed Parameters Collapsing header
			outputPins[OutputPin_CollHdrSpecies] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Asset, this);//Reactions Collapsing header

			collapsingHeadersIds[CollapsingHeader_Equations] = Widget::MNBV::GetMNBVCtxtNextId();//Species Collapsing header
			collapsingHeadersIds[CollapsingHeader_Reactions] = Widget::MNBV::GetMNBVCtxtNextId();//Simple Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_Parameters] = Widget::MNBV::GetMNBVCtxtNextId();//Computed Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_Species] = Widget::MNBV::GetMNBVCtxtNextId();//Reactions Collapsing header

			//Initialize the list boxes data
			nlbsData[NodeListBoxString_Equations] = { &data->GetAllEquation(), Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_Reactions] = { &data->GetAllReaction(), Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_Parameters] = { &data->GetAllParameter(), Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_Species] = { &data->GetAllSpecies(), Widget::MNBV::GetMNBVCtxtNextId() };

		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in asset node data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in asset node data

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in asset node data

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in asset node data

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::Equation.
	*/
	struct EquationNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_CollHdrModelLinks,
			InputPin_CollHdrEquations,
			InputPin_CollHdrKineticLaws,
			InputPin_CollHdrDataFields,
			InputPin_CollHdrEquationOperands,
			InputPin_NLBSEquations,
			InputPin_NLBSParameters,
			InputPin_NLBSSpecies,
			InputPin_EquationValue,
			InputPin_Asset,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_CollHdrModelLinks,
			OutputPin_CollHdrEquations,
			OutputPin_CollHdrKineticLaws,
			OutputPin_CollHdrDataFields,
			OutputPin_CollHdrEquationOperands,
			OutputPin_NLBSEquations,
			OutputPin_NLBSParameters,
			OutputPin_NLBSSpecies,
			OutputPin_EquationValue,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_ModelLinks,
			CollapsingHeader_Equations,
			CollapsingHeader_KineticLaws,
			CollapsingHeader_DataFields,
			CollapsingHeader_EquationOperands,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage access to the list boxes storing strings.
		@see ::nlbsData
		*/
		enum NodeListBoxString
		{
			NodeListBoxString_EquationOperands,
			NodeListBoxString_ParameterOperands,
			NodeListBoxString_SpeciesOperands,

			NodeListBoxString_Count
		};

		/*!
		@brief The local enum to manage to the encoding of the state of this
				node.
		@see ::utilityState
		*/
		enum State
		{
			State_CollHdrModelLinks,
			State_CollHdrEquations,
			State_CollHdrKineticLaws,
			State_CollHdrDataFields,
			State_CollHdrEquationOperands,

			State_Count
		};

		/*!
		@brief Pointer to the computed parameter represented by this node.
		*/
		std::shared_ptr<ECellEngine::Maths::Equation> data;

		/*!
		@brief The copy of the value of the Operation in the right hand side of
				the equation ::data.
		@remarks This is needed because the accessor to the value of lhs of the
				 equation works by copy.
		*/
		float lhsValueBuffer = 0.0f;

		/*!
		@brief Pointer to the dependency database of the simulation this node
				part of.
		*/
		const ECellEngine::Data::DependenciesDatabase* depDB;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		/*!
		@brief The byte the encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned short utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

		/*!
		@brief All the list boxes to store/display strings.
		@details Access the pins with the enum values NodeListBoxString_XXX
		*/
		NodeListBoxStringData<std::string> nlbsData[NodeListBoxString_Count];
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Maths::Equation>> nlbsDataEqDep;
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::Reaction>> nlbsDataRKLDep;
		std::vector<std::weak_ptr<ECellEngine::Maths::Equation>> equationDep;
		std::vector<std::weak_ptr<ECellEngine::Data::Reaction>> reactionKLDep;
		std::vector<std::string> speciesOperands;
		std::vector<std::string> parametersOperands;
		std::vector<std::string> equationsOperands;

		EquationNodeData(const EquationNodeData& _cpnd) :
			NodeData(_cpnd), data{ _cpnd.data }, depDB{ _cpnd.depDB },
			inputPins{ _cpnd.inputPins[0], _cpnd.inputPins[1] , _cpnd.inputPins[2] ,
					  _cpnd.inputPins[3] , _cpnd.inputPins[4] , _cpnd.inputPins[5],
					  _cpnd.inputPins[6] , _cpnd.inputPins[7] , _cpnd.inputPins[8], _cpnd.inputPins[9] },
			outputPins{ _cpnd.outputPins[0], _cpnd.outputPins[1] , _cpnd.outputPins[2] ,
					  _cpnd.outputPins[3] , _cpnd.outputPins[4] , _cpnd.outputPins[5],
					  _cpnd.outputPins[6] , _cpnd.outputPins[7] , _cpnd.outputPins[8] },
			utilityState{ _cpnd.utilityState },
			collapsingHeadersIds{ _cpnd.collapsingHeadersIds[0], _cpnd.collapsingHeadersIds[1] , _cpnd.collapsingHeadersIds[2] ,
					  _cpnd.collapsingHeadersIds[3] , _cpnd.collapsingHeadersIds[4] },
			nlbsData{ _cpnd.nlbsData[0], _cpnd.nlbsData[1] , _cpnd.nlbsData[2] },
			nlbsDataEqDep{ _cpnd.nlbsDataEqDep }, nlbsDataRKLDep{ _cpnd.nlbsDataRKLDep },
			equationDep{ _cpnd.equationDep },
			reactionKLDep{ _cpnd.reactionKLDep },
			speciesOperands{ _cpnd.speciesOperands },
			parametersOperands{ _cpnd.parametersOperands },
			equationsOperands{ _cpnd.equationsOperands }
		{
			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}

			nlbsDataEqDep.data = &equationDep;
			nlbsDataRKLDep.data = &reactionKLDep;
			nlbsData[NodeListBoxString_SpeciesOperands].data = &speciesOperands;
			nlbsData[NodeListBoxString_ParameterOperands].data = &parametersOperands;
			nlbsData[NodeListBoxString_EquationOperands].data = &equationsOperands;
		}

		EquationNodeData(std::shared_ptr<ECellEngine::Maths::Equation> _data, const ECellEngine::Data::DependenciesDatabase* _depDB) :
			NodeData(), data{ _data }, depDB{ _depDB }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			inputPins[InputPin_CollHdrEquations] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			inputPins[InputPin_CollHdrKineticLaws] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			inputPins[InputPin_CollHdrDataFields] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			inputPins[InputPin_CollHdrEquationOperands] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Equation Operands collapsing header
			inputPins[InputPin_NLBSEquations] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Computed Parameter Operands
			inputPins[InputPin_NLBSParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Simple Parameter Operands
			inputPins[InputPin_NLBSSpecies] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Node String List Box for Species Operands
			inputPins[InputPin_EquationValue] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Equation, this);//Operation Value Float
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Asset, this);//Asset

			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			outputPins[OutputPin_CollHdrEquations] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			outputPins[OutputPin_CollHdrKineticLaws] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			outputPins[OutputPin_CollHdrDataFields] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			outputPins[OutputPin_CollHdrEquationOperands] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Equation Operands collapsing header
			outputPins[OutputPin_NLBSEquations] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Computed Parameter Operands
			outputPins[OutputPin_NLBSParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Simple Parameter Operands
			outputPins[OutputPin_NLBSSpecies] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Node String List Box for Species Operands
			outputPins[OutputPin_EquationValue] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_EquationValueFloat, this);//Operation Value Float

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//ModelLinks Collapsing header
			collapsingHeadersIds[CollapsingHeader_Equations] = Widget::MNBV::GetMNBVCtxtNextId();//Computed Parameters section
			collapsingHeadersIds[CollapsingHeader_KineticLaws] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Laws section
			collapsingHeadersIds[CollapsingHeader_DataFields] = Widget::MNBV::GetMNBVCtxtNextId();//Data Fields collapsing header
			collapsingHeadersIds[CollapsingHeader_EquationOperands] = Widget::MNBV::GetMNBVCtxtNextId();//Equation Operands collapsing header

			auto search = depDB->GetEquationDependencies().find(data);
			if (search != depDB->GetEquationDependencies().end())
			{
				equationDep = search->second.partOfEquation;
				reactionKLDep = search->second.partOfReactionKineticLaw;
			}
			else
			{
				equationDep = {};
				reactionKLDep = {};
			}
			nlbsDataEqDep = { &equationDep , Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsDataRKLDep = { &reactionKLDep , Widget::MNBV::GetMNBVCtxtNextId() };
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);//Node String List Box for Species Operands
			nlbsData[NodeListBoxString_SpeciesOperands] = { &speciesOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Node String List Box for Species Operands
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::Parameter>(parametersOperands);//Node String List Box for Simple Parameter Operands
			nlbsData[NodeListBoxString_ParameterOperands] = { &parametersOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Node String List Box for Simple Parameter Operands
			_data->GetOperation().GetOperandsNames<ECellEngine::Maths::Equation>(equationsOperands);//Node String List Box for Computed Parameter Operands
			nlbsData[NodeListBoxString_EquationOperands] = { &equationsOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Node String List Box for Computed Parameter Operands
		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in equation data

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in equation data

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in equation data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;

		/*!
		@brief Updates the data that this node represents (::lhsValueBuffer).
		*/
		void Update() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node to set up and
			display a line plot.
	*/
	struct LinePlotNodeData final : public NodeData
	{
	public:
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_CollHdrPlot,
			InputPin_XAxis,
			InputPin_YAxis,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_None,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_AllParameters,
			CollapsingHeader_GeneralParameters,
			CollapsingHeader_PlotFlags,
			CollapsingHeader_XAxisFlags,
			CollapsingHeader_YAxisFlags,
			CollapsingHeader_AxisScaleFlags,
			CollapsingHeader_Plot,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage to the encoding of the state of this
				node.
		@see ::utilityState
		*/
		enum State
		{
			State_CollHdrAllParameters,
			State_CollHdrGeneralParameters,
			State_CollHdrPlotFlags,
			State_CollHdrXAxisFlags,
			State_CollHdrYAxisFlags,
			State_CollHdrAxisScaleFlags,
			State_CollHdrPlot,
			State_PlotIsOpen,

			State_Count
		};

		Plot::LinePlot linePlot;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];//not used

		/*!
		@brief The 2 bytes to encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned short utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

		LinePlotNodeData(ImVec2& _position) :
			NodeData()
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_CollHdrPlot] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Plot Collapsing Header
			inputPins[InputPin_XAxis] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackSubscriber, this);//X
			inputPins[InputPin_YAxis] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackSubscriber, this);//Y

			outputPins[OutputPin_None] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this); //not used

			collapsingHeadersIds[CollapsingHeader_AllParameters] = Widget::MNBV::GetMNBVCtxtNextId();//Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_GeneralParameters] = Widget::MNBV::GetMNBVCtxtNextId();//General (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_PlotFlags] = Widget::MNBV::GetMNBVCtxtNextId();//Plot Flags (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_XAxisFlags] = Widget::MNBV::GetMNBVCtxtNextId();//X Axis Flags (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_YAxisFlags] = Widget::MNBV::GetMNBVCtxtNextId();//Y Axis Flags (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_AxisScaleFlags] = Widget::MNBV::GetMNBVCtxtNextId();//X & Y Axis Scale Flags (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_Plot] = Widget::MNBV::GetMNBVCtxtNextId();//Plot Collapsing Header
		}

		LinePlotNodeData(const LinePlotNodeData& _lpnd) :
			NodeData(_lpnd), linePlot{ _lpnd.linePlot },
			inputPins{ _lpnd.inputPins[0], _lpnd.inputPins[1] , _lpnd.inputPins[2] },
			outputPins{ _lpnd.outputPins[0] },
			utilityState{ _lpnd.utilityState },
			collapsingHeadersIds{ _lpnd.collapsingHeadersIds[0], _lpnd.collapsingHeadersIds[1],
			_lpnd.collapsingHeadersIds[2], _lpnd.collapsingHeadersIds[3], _lpnd.collapsingHeadersIds[4],
			_lpnd.collapsingHeadersIds[5] }
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

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};// not used in line plot node data

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};// not used in line plot node data

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};// not used in line plot node data

		inline bool IsPlotOpen() noexcept
		{
			return (utilityState >> State_PlotIsOpen) & 1;
		}

		/*!
		@brief Utility function to switch the value 0 -> 1 or 1 -> 0 of the bit
				at position @p _stateBitPos in ::utilityState.
		@remarks It could be moved outside of this class in the future since the
				 code works with any data and is not pecific to this context.
		*/
		inline void SwitchState(const short _stateBitPos) noexcept
		{
			utilityState ^= (1 << _stateBitPos);
		}
	};

	/*!
	@brief The logic to encode the data needed to draw the node to define and
			use a logic operation.
	*/
	struct LogicOperationNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_LHS,
			InputPin_RHS,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_OnOperandChange,
			OutputPin_OnResultChange,

			OutputPin_Count
		};

		/*!
		@brief The display list of the logical operators.
		*/
		const char* operatorTypes[3] = { "AND", "OR", "NOT"};

		/*!
		@brief The placeholder value of the left hand side of the logical operation.
		@details It is used both as a buffer to display the value in the node and the
				 placeholder value when nothing is connected to the corresponding
				 input pin.
		*/
		bool lhs = false;

		/*!
		@brief The placeholder value of the right hand side of the logical operation.
		@details It is used both as a buffer to display the value in the node and the
				 placeholder value when nothing is connected to the corresponding
				 input pin.
		*/
		bool rhs = false;

		/*!
		@brief Pointer to the logical operation represented by this node.
		*/
		std::shared_ptr<Maths::LogicOperation> data;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		LogicOperationNodeData(std::shared_ptr<Maths::LogicOperation> _data, ImVec2& _position) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_LHS] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackSubscriber, this); //Left hand side
			inputPins[InputPin_RHS] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackSubscriber, this); //Right hand side
			outputPins[OutputPin_OnOperandChange] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackPublisher, this); //Callback output pin when an input value (operand) changes
			outputPins[OutputPin_OnResultChange] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackPublisher, this); //Callback output pin when the result changes
		}

		LogicOperationNodeData(const LogicOperationNodeData& _lond) :
			NodeData(_lond),
			lhs{ _lond.lhs }, rhs{ _lond.rhs }, data{ _lond.data },
			inputPins{ _lond.inputPins[0], _lond.inputPins[1] },
			outputPins{ _lond.outputPins[0], _lond.outputPins[1]}
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

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Logic Operation Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in Logic Operation Node Data
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Core::Events::ModifyDataStateValueEvent.
	*/
	struct ModifyDataStateValueEventNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_FloatValue,
			InputPin_Condition,
			InputPin_CollHdrExecution,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_Modify,
			OutputPin_CollHdrExecution,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_Execution,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage to the encoding of the state of this
				node.
		@see ::utilityState
		*/
		enum State
		{
			State_CollHdrExecution,

			State_Count
		};

		/*!
		@brief Pointer to the event represented by this node.
		*/
		std::shared_ptr<ECellEngine::Core::Events::ModifyDataStateValueEvent> data;

		/*!
		@brief A boolean to control whether the user can use a button to
				manually execute the event.
		*/
		bool activateManualExecution = false;

		/*!
		@brief The value to use when the  event is executed if no link is
				connected to the input pin.
		*/
		float value = 0.0f;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		/*!
		@brief The 2 bytes to encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned short utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

		ModifyDataStateValueEventNodeData(const ModifyDataStateValueEventNodeData& _mdstvend) :
			NodeData(_mdstvend), data{ _mdstvend.data },
			inputPins{ _mdstvend.inputPins[0], _mdstvend.inputPins[1], _mdstvend.inputPins[2] },
			outputPins{ _mdstvend.outputPins[0], _mdstvend.outputPins[1] },
			collapsingHeadersIds{ _mdstvend.collapsingHeadersIds[0] }
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

		ModifyDataStateValueEventNodeData(std::shared_ptr<ECellEngine::Core::Events::ModifyDataStateValueEvent> _data, ImVec2& _position) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_FloatValue] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackSubscriber, this);//the new value we will use to modify the data state
			inputPins[InputPin_Condition] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackSubscriber, this);//the watchers that will trigger this event
			inputPins[InputPin_CollHdrExecution] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//the Execution Collapsing Header
			
			outputPins[OutputPin_Modify] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FreeValueFloat, this);//Connection to the value to modify
			outputPins[OutputPin_CollHdrExecution] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//the Execution Collapsing Header

			collapsingHeadersIds[CollapsingHeader_Execution] = Widget::MNBV::GetMNBVCtxtNextId();
		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Modify DataState Value Event Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in Modify DataState Value Event Node Data
	};
	
	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::Reaction.
	*/
	struct ReactionNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_CollHdrModelLinks,
			InputPin_CollHdrReactants,
			InputPin_CollHdrProducts,
			InputPin_CollHdrKineticLaw,
			InputPin_CollHdrKineticLawOperands,
			InputPin_NLBSEquations,
			InputPin_NLBSParameters,
			InputPin_NLBSSpecies,
			InputPin_KineticLawValue,
			InputPin_Asset,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_CollHdrModelLinks,
			OutputPin_CollHdrReactants,
			OutputPin_CollHdrProducts,
			OutputPin_CollHdrKineticLaw,
			OutputPin_CollHdrKineticLawOperands,
			OutputPin_NLBSEquations,
			OutputPin_NLBSParameters,
			OutputPin_NLBSSpecies,
			OutputPin_KineticLawValue,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_ModelLinks,
			CollapsingHeader_Reactants,
			CollapsingHeader_Products,
			CollapsingHeader_KineticLaw,
			CollapsingHeader_KineticLawOperands,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage access to the list boxes storing strings.
		@see ::nlbsData
		*/
		enum NodeListBoxString
		{
			NodeListBoxString_Reactants,
			NodeListBoxString_Products,
			NodeListBoxString_EquationOperands,
			NodeListBoxString_ParameterOperands,
			NodeListBoxString_SpeciesOperands,

			NodeListBoxString_Count
		};

		/*!
		@brief The local enum to manage to the encoding of the state of this
				node.
		@see ::utilityState
		*/
		enum State
		{
			State_CollHdrModelLinks,
			State_CollHdrReactants,
			State_CollHdrProducts,
			State_CollHdrKineticLaw,
			State_CollHdrKineticLawOperands,

			State_Count
		};

		/*!
		@brief Pointer to the reaction represented by this node.
		*/
		ECellEngine::Data::Reaction* data;

		/*!
		@brief The local copy of ECellEngine::Data::Reaction::kineticLawValueCache of ::data.
		@remarks This is needed because the accessor to the kinetic law value
				 returns a copy of the value and not a pointer.
		*/
		float kineticLawValueBuffer = 0.0f;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		/*!
		@brief The byte the encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned short utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];


		/*!
		@brief All the list boxes to store/display strings.
		@details Access the pins with the enum values NodeListBoxString_XXX
		*/
		NodeListBoxStringData<std::string> nlbsData[NodeListBoxString_Count];
		std::vector<std::string> speciesOperands;
		std::vector<std::string> parametersOperands;
		std::vector<std::string> equationsOperands;

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
			parametersOperands{ _rnd.parametersOperands },
			equationsOperands{ _rnd.equationsOperands }
		{
			nlbsData[NodeListBoxString_SpeciesOperands].data = &speciesOperands;
			nlbsData[NodeListBoxString_ParameterOperands].data = &parametersOperands;
			nlbsData[NodeListBoxString_EquationOperands].data = &equationsOperands;

			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}
		}

		ReactionNodeData(ECellEngine::Data::Reaction* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			inputPins[InputPin_CollHdrReactants] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactants section Collapsing header
			inputPins[InputPin_CollHdrProducts] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Products section Collapsing header
			inputPins[InputPin_CollHdrKineticLaw] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Collapsing header
			inputPins[InputPin_CollHdrKineticLawOperands] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Operands Collapsing header
			inputPins[InputPin_NLBSEquations] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameter Operands from Kinetic Law
			inputPins[InputPin_NLBSParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Simple Parameter Operands from Kinetic Law
			inputPins[InputPin_NLBSSpecies] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Species Operands from Kinetic Law
			inputPins[InputPin_KineticLawValue] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Equation, this);//Kinetic Law Value Float field (must be Read Only)
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Reaction, this);//Asset

			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			outputPins[OutputPin_CollHdrReactants] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactants section Collapsing header
			outputPins[OutputPin_CollHdrProducts] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Products section Collapsing header
			outputPins[OutputPin_CollHdrKineticLaw] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Collapsing header
			outputPins[OutputPin_CollHdrKineticLawOperands] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Operands Collapsing header
			outputPins[OutputPin_NLBSEquations] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameter Operands from Kinetic Law
			outputPins[OutputPin_NLBSParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Simple Parameter Operands from Kinetic Law
			outputPins[OutputPin_NLBSSpecies] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Species Operands from Kinetic Law
			outputPins[OutputPin_KineticLawValue] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_EquationValueFloat, this);//Kinetic Law Value Float field (must be Read Only)

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//ModelLinks Collapsing header
			collapsingHeadersIds[CollapsingHeader_Reactants] = Widget::MNBV::GetMNBVCtxtNextId();//Reactants section Collapsing header
			collapsingHeadersIds[CollapsingHeader_Products] = Widget::MNBV::GetMNBVCtxtNextId();//Products section Collapsing header
			collapsingHeadersIds[CollapsingHeader_KineticLaw] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Law collapsing header
			collapsingHeadersIds[CollapsingHeader_KineticLawOperands] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Law Operands collapsing header

			nlbsData[NodeListBoxString_Reactants] = { _data->GetReactants() , Widget::MNBV::GetMNBVCtxtNextId() };//Reactants section
			nlbsData[NodeListBoxString_Products] = { data->GetProducts(), Widget::MNBV::GetMNBVCtxtNextId() };//Products section
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);
			nlbsData[NodeListBoxString_SpeciesOperands] = { &speciesOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Species Operands from Kinetic Law
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::Parameter>(parametersOperands);
			nlbsData[NodeListBoxString_ParameterOperands] = { &parametersOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Simple Parameter Operands from Kinetic Law
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Maths::Equation>(equationsOperands);
			nlbsData[NodeListBoxString_EquationOperands] = { &equationsOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Computed Parameter Operands from Kinetic Law
		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Reaction Node Data

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Reaction Node Data

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Reaction Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;

		/*!
		@brief Updates the ::kineticLawValueBuffer.
		*/
		void Update() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::Parameter.
	*/
	struct ParameterNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_CollHdrModelLinks,
			InputPin_CollHdrEquations,
			InputPin_CollHdrKineticLaws,
			InputPin_CollHdrDataFields,
			InputPin_ParameterValue,
			InputPin_Asset,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_ThisData,
			OutputPin_CollHdrModelLinks,
			OutputPin_CollHdrEquations,
			OutputPin_CollHdrKineticLaws,
			OutputPin_CollHdrDataFields,
			OutputPin_ParameterValue,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_ModelLinks,
			CollapsingHeader_Equations,
			CollapsingHeader_KineticLaws,
			CollapsingHeader_DataFields,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage to the encoding of the state of this
				node.
		@see ::utilityState
		*/
		enum State
		{
			State_CollHdrModelLinks,
			State_CollHdrEquations,
			State_CollHdrKineticLaws,
			State_CollHdrDataFields,

			State_Count
		};

		/*!
		@brief Pointer to the simple parameter represented by this node.
		*/
		std::shared_ptr<ECellEngine::Data::Parameter> data;

		/*!
		@brief The copy of ECellEngine::Data::Parameter::value.
		@remarks This is needed because the accessor to the value works by copy.
		*/
		float parameterValueBuffer = 0.f;

		/*!
		@brief Pointer to the dependency database of the simulation this node
				part of.
		*/
		const ECellEngine::Data::DependenciesDatabase* depDB;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		/*!
		@brief The byte the encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned short utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

		/*!
		@brief All the list boxes to store/display strings.
		@details Access the pins with the enum values NodeListBoxString_XXX
		*/
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Maths::Equation>> nlbsDataEqDep;
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::Reaction>> nlbsDataRKLDep;
		std::vector<std::weak_ptr<ECellEngine::Maths::Equation>> equationDep;
		std::vector<std::weak_ptr<ECellEngine::Data::Reaction>> reactionKLDep;

		ParameterNodeData(const ParameterNodeData& _pnd) :
			NodeData(_pnd), data{ _pnd.data }, depDB{ _pnd.depDB },
			inputPins{ _pnd.inputPins[0], _pnd.inputPins[1] , _pnd.inputPins[2] ,
					  _pnd.inputPins[3] , _pnd.inputPins[4] , _pnd.inputPins[5] },
			outputPins{ _pnd.outputPins[0], _pnd.outputPins[1] , _pnd.outputPins[2] ,
					  _pnd.outputPins[3] , _pnd.outputPins[4], _pnd.outputPins[5]},
			utilityState{ _pnd.utilityState },
			collapsingHeadersIds{ _pnd.collapsingHeadersIds[0], _pnd.collapsingHeadersIds[1] , _pnd.collapsingHeadersIds[2] ,
					  _pnd.collapsingHeadersIds[3] },
			nlbsDataEqDep{ _pnd.nlbsDataEqDep }, nlbsDataRKLDep{ _pnd.nlbsDataRKLDep },
			equationDep{ _pnd.equationDep }, reactionKLDep{ _pnd.reactionKLDep }
		{
			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}

			nlbsDataEqDep.data = &equationDep;
			nlbsDataRKLDep.data = &reactionKLDep;
		}

		ParameterNodeData(std::shared_ptr<ECellEngine::Data::Parameter> _data, const ECellEngine::Data::DependenciesDatabase* _depDB) :
			NodeData(), data{ _data }, depDB{ _depDB }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));


			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			inputPins[InputPin_CollHdrEquations] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Computed Parameters section
			inputPins[InputPin_CollHdrKineticLaws] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Laws section
			inputPins[InputPin_CollHdrDataFields] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			inputPins[InputPin_ParameterValue] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackSubscriber, this);//Value Float field
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Asset, this);//Asset

			outputPins[OutputPin_ThisData] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//This Data
			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			outputPins[OutputPin_CollHdrEquations] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Computed Parameters section
			outputPins[OutputPin_CollHdrKineticLaws] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Laws section
			outputPins[OutputPin_CollHdrDataFields] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			outputPins[OutputPin_ParameterValue] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackPublisher, this);//Value Float field

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//ModelLinks Collapsing header
			collapsingHeadersIds[CollapsingHeader_Equations] = Widget::MNBV::GetMNBVCtxtNextId();//Computed Parameters section
			collapsingHeadersIds[CollapsingHeader_KineticLaws] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Laws section
			collapsingHeadersIds[CollapsingHeader_DataFields] = Widget::MNBV::GetMNBVCtxtNextId();//Data Fields collapsing header

			auto search = depDB->GetParameterDependencies().find(data);
			if (search != depDB->GetParameterDependencies().end())
			{
				equationDep = search->second.partOfEquation;
				reactionKLDep = search->second.partOfReactionKineticLaw;
			}
			else
			{
				equationDep = {};
				reactionKLDep = {};
			}

			nlbsDataEqDep = { &equationDep, Widget::MNBV::GetMNBVCtxtNextId() }; //Computed Parameters section
			nlbsDataRKLDep = { &reactionKLDep, Widget::MNBV::GetMNBVCtxtNextId() };//Kinetic Laws section
		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override ;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override ;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Parameter Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;

		/*!
		@brief Updates the buffer of the parameter value.
		*/
		void Update() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node to access the
			ECellEngine::Core::Timer.
	*/
	struct SimulationTimeNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_None,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_SimulationTime,

			OutputPin_Count
		};

		/*!
		@brief Pointer to the timer partially represented by this node.
		*/
		ECellEngine::Core::Timer* simulationTimer;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		SimulationTimeNodeData(ECellEngine::Core::Timer* _simulationTimer, ImVec2& _position) :
			NodeData(), simulationTimer{ _simulationTimer }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_None] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this); //not used
			outputPins[OutputPin_SimulationTime] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackPublisher, this); //simulation Time
		}

		SimulationTimeNodeData(const SimulationTimeNodeData& _stnd) :
			NodeData(_stnd), simulationTimer{ _stnd.simulationTimer },
			inputPins{ _stnd.inputPins[0] },
			outputPins{ _stnd.outputPins[0] }
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

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Simulation Time Node Data

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Simulation Time Node Data

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Simulation Time Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in Simulation Time Node Data
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::Solver.
	*/
	struct SolverNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_None,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_Solver,

			OutputPin_Count
		};

		/*!
		@brief Pointer to the solver represented by this node.
		*/
		std::shared_ptr<ECellEngine::Solvers::Solver> data;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		SolverNodeData(const SolverNodeData& _slvnd) :
			NodeData(_slvnd), data{ _slvnd.data },
			inputPins{ _slvnd.inputPins[0] },
			outputPins{ _slvnd.outputPins[0] }
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

		SolverNodeData(std::shared_ptr<ECellEngine::Solvers::Solver> _data, ImVec2& _position) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_None] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//not used
			outputPins[OutputPin_Solver] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Solver, this);//this solver transmission

		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Solver Node Data

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Solver Node Data

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Solver Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;
	};
	
	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::Species.
	*/
	struct SpeciesNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_CollHdrModelLinks,
			InputPin_CollHdrDataFields,
			InputPin_CollHdrAsReactant,
			InputPin_CollHdrAsProduct,
			InputPin_CollHdrInEquation,
			InputPin_CollHdrInKineticLaw,
			InputPin_Quantity,
			InputPin_Asset,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_ThisData,
			OutputPin_CollHdrModelLinks,
			OutputPin_CollHdrDataFields,
			OutputPin_CollHdrAsReactant,
			OutputPin_CollHdrAsProduct,
			OutputPin_CollHdrInEquation,
			OutputPin_CollHdrInKineticLaw,
			OutputPin_Quantity,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_ModelLinks,
			CollapsingHeader_AsReactant,
			CollapsingHeader_AsProduct,
			CollapsingHeader_InEquation,
			CollapsingHeader_InKineticLaw,
			CollapsingHeader_DataFields,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage to the encoding of the state of this
				node.
		@see ::utilityState
		*/
		enum State
		{
			State_CollHdrModelLinks,
			State_CollHdrAsReactant,
			State_CollHdrAsProduct,
			State_CollHdrInEquation,
			State_CollHdrInKineticLaw,
			State_CollHdrDataFields,

			State_Count
		};

		/*!
		@brief Pointer to the species represented by this node.
		*/
		std::shared_ptr<ECellEngine::Data::Species> data;

		/*!
		@brief The copy of ECellEngine::Data::Species::quantity.
		@remarks This is needed because the accessor to the value works by copy.
		*/
		float speciesQuantityBuffer = 0.f;

		/*!
		@brief Pointer to the dependency database of the simulation this node
				part of.
		*/
		const ECellEngine::Data::DependenciesDatabase* depDB;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		/*!
		@brief The byte the encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned short utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

		/*!
		@brief All the list boxes to store/display strings.
		@details Access the pins with the enum values NodeListBoxString_XXX
		*/
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Maths::Equation>> nlbsDataEqDep;//computed parameter dep
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::Reaction>> nlbsDataRRDep;//reaction reactants dep
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::Reaction>> nlbsDataRPDep;//reaction products dep
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::Reaction>> nlbsDataRKLDep;//reaction kinetic law dep
		std::vector<std::weak_ptr<ECellEngine::Maths::Equation>> equationDep;
		std::vector<std::weak_ptr<ECellEngine::Data::Reaction>> reactionRDep;//reaction reactants dep
		std::vector<std::weak_ptr<ECellEngine::Data::Reaction>> reactionPDep;//reaction products dep
		std::vector<std::weak_ptr<ECellEngine::Data::Reaction>> reactionKLDep;

		SpeciesNodeData(const SpeciesNodeData& _snd) :
			NodeData(_snd), data{ _snd.data }, depDB{ _snd.depDB },
			speciesQuantityBuffer{ _snd.speciesQuantityBuffer },
			inputPins{ _snd.inputPins[0], _snd.inputPins[1] , _snd.inputPins[2] ,
					  _snd.inputPins[3] , _snd.inputPins[4] , _snd.inputPins[5],
					  _snd.inputPins[6] , _snd.inputPins[7] },
			outputPins{ _snd.outputPins[0], _snd.outputPins[1] , _snd.outputPins[2] ,
					  _snd.outputPins[3] , _snd.outputPins[4] , _snd.outputPins[5],
					  _snd.outputPins[6], _snd.outputPins[7]},
			utilityState{ _snd.utilityState },
			collapsingHeadersIds{ _snd.collapsingHeadersIds[0], _snd.collapsingHeadersIds[1],
			_snd.collapsingHeadersIds[2], _snd.collapsingHeadersIds[3], _snd.collapsingHeadersIds[4],
			_snd.collapsingHeadersIds[5] },
			nlbsDataEqDep{ _snd.nlbsDataEqDep }, nlbsDataRRDep{ _snd.nlbsDataRRDep },
			nlbsDataRPDep{ _snd.nlbsDataRPDep }, nlbsDataRKLDep{ _snd.nlbsDataRKLDep },
			equationDep{ _snd.equationDep }, reactionRDep{ _snd.reactionRDep },
			reactionPDep{ _snd.reactionPDep }, reactionKLDep{ _snd.reactionKLDep }
		{
			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}

			nlbsDataEqDep.data = &equationDep;
			nlbsDataRRDep.data = &reactionRDep;
			nlbsDataRPDep.data = &reactionPDep;
			nlbsDataRKLDep.data = &reactionKLDep;
		}

		SpeciesNodeData(std::shared_ptr<ECellEngine::Data::Species> _data, const ECellEngine::Data::DependenciesDatabase* _depDB) :
			NodeData(), data{ _data }, depDB{ _depDB }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Model Links
			inputPins[InputPin_CollHdrDataFields] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Data Fields
			inputPins[InputPin_CollHdrAsReactant] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactions's Reactants
			inputPins[InputPin_CollHdrAsProduct] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Products
			inputPins[InputPin_CollHdrInEquation] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Computed parameters equation
			inputPins[InputPin_CollHdrInKineticLaw] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Kinetic Law
			inputPins[InputPin_Quantity] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackSubscriber, this);//Quantity
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Asset

			outputPins[OutputPin_ThisData] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//The Species data in this node
			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Model Links
			outputPins[OutputPin_CollHdrDataFields] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Data Fields
			outputPins[OutputPin_CollHdrAsReactant] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactions's Reactants
			outputPins[OutputPin_CollHdrAsProduct] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Products
			outputPins[OutputPin_CollHdrInEquation] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Computed parameters equation
			outputPins[OutputPin_CollHdrInKineticLaw] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Kinetic Law
			outputPins[OutputPin_Quantity] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackPublisher, this);//Quantity

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Model Links
			collapsingHeadersIds[CollapsingHeader_InEquation] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Computed parameters equation
			collapsingHeadersIds[CollapsingHeader_AsReactant] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Reactions's Reactants
			collapsingHeadersIds[CollapsingHeader_AsProduct] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Reaction's Products
			collapsingHeadersIds[CollapsingHeader_InKineticLaw] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Reaction's Kinetic Law
			collapsingHeadersIds[CollapsingHeader_DataFields] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Data Fields

			auto search = depDB->GetSpeciesDependencies().find(data);
			if (search != depDB->GetSpeciesDependencies().end())
			{
				equationDep = search->second.partOfEquation;
				reactionRDep = search->second.partOfReactionAsReactant;
				reactionPDep = search->second.partOfReactionAsProduct;
				reactionKLDep = search->second.partOfReactionKineticLaw;
			}
			else
			{
				equationDep = {};
				reactionKLDep = {};
			}

			nlbsDataEqDep = { &equationDep, Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsDataRRDep = { &reactionRDep, Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsDataRPDep = { &reactionPDep, Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsDataRKLDep = { &reactionKLDep, Widget::MNBV::GetMNBVCtxtNextId() };
		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Species Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override ;

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;

		/*!
		@brief Updates the buffer of the parameter value.
		*/
		void Update() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node to define and
			use a custom float.
	*/
	struct ValueFloatNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_None,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_Value,

			OutputPin_Count
		};

		/*!
		@brief Stores the float value.
		@details We chose to store the value in a parameter to access
				 callback compliant datastructure and because it is the
				 less invasive way to currently to that.
		*/
		Data::Parameter value;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		ValueFloatNodeData(float _value, ImVec2& _position) :
			NodeData(),
			value{ "ValueFloatNode[" + std::to_string((std::size_t)id) + "]::Value", _value }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			value.name = "ValueFloatNode[" + std::to_string((std::size_t)id) + "]::Value";

			inputPins[InputPin_None] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this); //not used
			outputPins[OutputPin_Value] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_FloatCallBackPublisher, this); //Value
		}

		ValueFloatNodeData(const ValueFloatNodeData& _vfnd) :
			NodeData(_vfnd), value{ _vfnd.value },
			inputPins{ _vfnd.inputPins[0] },
			outputPins{ _vfnd.outputPins[0] }
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

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Value Float Node Data

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Value Float Node Data

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override {};//not used in Value Float Node Data

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;
	
		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in Value Float Node Data
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Core::Trigger.
	*/
	struct TriggerNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_Target,
			InputPin_Threshold,

			InputPin_Count
		};

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
		enum OutputPin
		{
			OutputPin_OnTriggerEnter,
			OutputPin_OnTriggerStay,
			OutputPin_OnTriggerExit,

			OutputPin_Count
		};

		/*!
		@brief Pointer to the watcher represented by this node.
		*/
		std::shared_ptr<ECellEngine::Core::Trigger<Operand*, Operand*>> data;

		/*!
		@brief The default operand to hold a value in this watcher when nothing
				is connected in the input pin of the target.
		@details We use the class of a parameter as the default operand because
				 it is the less intrusive type of operand which value can be
				 changed at runtime.
		@remarks From an editor point of view, this default value should never
			be used because it is not going to be updated if nothing is plugged
			in the matching input pin. So, it is mainly here as a placeholder.
		*/
		Data::Parameter target;

		/*!
		@brief The display list of the comparators.
		*/
		const char* comparators[6] = { ">", ">=", "==", "!=", "<", "<=" };

		/*!
		@brief The default operand to hold a value in this watcher when nothing
				is connected in the input pin of the threshold.
		@details We use the class of a parameter as the default operand because
				 it is the less intrusive type of operand which value can be
				 changed at runtime.
		*/
		Data::Parameter threshold;

		/*!
		@brief All the input pins.
		@details Access the pins with the enum values InputPin_XXX
		*/
		NodeInputPinData inputPins[InputPin_Count];

		/*!
		@brief All the output pins.
		@details Access the pins with the enum values OutputPin_XXX
		*/
		NodeOutputPinData outputPins[OutputPin_Count];

		TriggerNodeData(const TriggerNodeData& _wtnd) :
			NodeData(_wtnd), data{ _wtnd.data },
			inputPins{ _wtnd.inputPins[0], _wtnd.inputPins[1] },
			outputPins{ _wtnd.outputPins[0], _wtnd.outputPins[1], _wtnd.outputPins[2] },
			target{ _wtnd.target },
			threshold{ _wtnd.threshold }
		{
			if (*_wtnd.data->GetTarget() == _wtnd.target)
			{
				data->SetTarget(&target);
			}
			else
			{
				data->SetTarget(_wtnd.data->GetTarget());
			}

			if (*_wtnd.data->GetThreshold() == _wtnd.threshold)
			{
				data->SetThreshold(&threshold);
			}
			else
			{
				data->SetThreshold(_wtnd.data->GetThreshold());
			}

			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}
		}

		TriggerNodeData(std::shared_ptr<ECellEngine::Core::Trigger<Operand*, Operand*>> _data, ImVec2& _position) :
			NodeData(), data{ _data },
			target{"Trigger["+std::to_string((std::size_t)id) + "]::Target", 0},
			threshold{"Trigger[" + std::to_string((std::size_t)id) + "]::Threshold", 0}
		{
			target.name = "Trigger[" + std::to_string((std::size_t)id) + "]::Target";
			data->SetTarget(&target);

			threshold.name = "Trigger[" + std::to_string((std::size_t)id) + "]::Threshold";
			data->SetThreshold(&threshold);

			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_Target] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Operand, this);//the Left Hand Side of the comparison
			inputPins[InputPin_Threshold] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Operand, this);//the Right Hand Side of the comparison
			outputPins[OutputPin_OnTriggerEnter] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackPublisher, this);//To all the event to trigger
			outputPins[OutputPin_OnTriggerStay] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackPublisher, this);//To all the event to trigger
			outputPins[OutputPin_OnTriggerExit] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_BooleanCallBackPublisher, this);//To all the event to trigger
		}

		void InputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void InputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput, void* _data) override;

		void OutputConnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputDisconnect(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override;

		void OutputRefresh(NodeInputPinData* _nodeInput, NodeOutputPinData* _nodeOutput) override {};//not used in Trigger Node Data
	};
#pragma endregion
}