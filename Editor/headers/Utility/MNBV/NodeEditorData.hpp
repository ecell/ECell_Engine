#pragma once
/*!
@file
@brief The source of the data structures describing nodes, pins and links
		that are used to know how to draw these entities in ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget
*/

#include "implot.h"
#include "imgui_node_editor.h"

#include "Data/BinaryOperatedVector.hpp"
#include "Core/Timer.hpp"
#include "Utility/PlotUtility.hpp"
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
		/*!
		@brief The id of this link
		*/
		ax::NodeEditor::LinkId id;

		/*!
		@brief The ids of the pins to use to as the start of the link.
		@details At index 0 is the main pin while index 1 contains the if of the
				 fallback pin.
		*/
		ax::NodeEditor::PinId startIds[2];

		/*!
		@brief The ids of the pins to use to use as the end of the link.
		@details At index 0 is the main pin while index 1 contains the if of the
				 fallback pin.
		*/
		ax::NodeEditor::PinId endIds[2];

		LinkData(ax::NodeEditor::PinId _startId, ax::NodeEditor::PinId _endId) :
			id{ Widget::MNBV::GetMNBVCtxtNextId() }, startIds{ _startId, _startId }, endIds{ _endId, _endId }
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
		@brief Method to implement what to do when a new link is connected
				to @p _nodeInput.
		@param _nodeInput The pin notifying this node that a link was made with
				it as the end.
		*/
		virtual void InputConnect(const NodeInputPinData& _nodeInput) = 0;

		/*!
		@brief Method to implement what to do when a pin (@p _nodeInput) received
				a char array as data.
		@param _nodeInput The pin notifying this node that a char array has arrived
				through it.
		*/
		virtual void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) = 0;

		/*!
		@brief Method to implement what to do when a pin (@p _nodeInput) received
				a float as data.
		@param _nodeInput The pin notifying this node that a float has arrived
				through it.
		*/
		virtual void InputUpdate(const NodeInputPinData& _nodeInput, float _data) = 0;

		/*!
		@brief Method to implement what to do when a new link is connected
				to @p _nodeOutput.
		@param _nodeInput The pin notifying this node that a link was made with
				it as the start.
		*/
		virtual void OutputConnect(const NodeOutputPinData& _nodeOutput) = 0;

		/*!
		@brief Method to implement what to do send data from a pin (@p _nodeOutput)
				through all of its connected links.
		@param _nodeOutput The start pin we want to broadcast data from.
		*/
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
		@brief What to do when the pin receives data.
		@tparam Data The type of data the pin receives. Supports char* and float.
		@param _data The data the pin receives from its publisher.
		*/
		template<class Data>
		inline void Receive(Data _data)
		{
			node->InputUpdate(*this, _data);
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
		/*!
		@brief The list of every input pins which shall receive the data when
				this output pin data broadcasts.
		*/
		std::vector<NodeInputPinData*> subscribers;

		NodeOutputPinData() = default;

		NodeOutputPinData(std::size_t& _pinId, PinType _type, NodeData* _node) :
			NodePinData(_pinId, ax::NodeEditor::PinKind::Output, _type, _node)
		{

		}

		/*!
		@brief The logic to add a new input pin as subscriber to this
				output pin.
		@details Typically happens every time a link is created.
		*/
		inline void AddSubscriber(NodeInputPinData* _newSubscriber)
		{
			subscribers.push_back(_newSubscriber);

			//What to do on connection.
			node->OutputConnect(*this);
			_newSubscriber->node->InputConnect(*_newSubscriber);

			//Send data for the first time
			node->OutputUpdate(*this);
		}

		/*!
		@brief Broadcasts data to the ::subscribers.
		@tparam Data The type of data the pin broadcasts. Supports char* and
				float on the side of the subscribers.
		@param _data The data the pin broadcasts to its ::subscribers.
		*/
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
			OutputPin_CollHdrComputedParameters,
			OutputPin_CollHdrReactions,
			OutputPin_CollHdrSimpleParameters,
			OutputPin_CollHdrSpecies,

			OutputPin_Count
		};

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_ComputedParameters,
			CollapsingHeader_Reactions,
			CollapsingHeader_SimpleParameters,
			CollapsingHeader_Species,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage access to the list boxes storing strings.
		@see ::nlbsData
		*/
		enum NodeListBoxString
		{
			NodeListBoxString_ComputedParameters,
			NodeListBoxString_Reactions,
			NodeListBoxString_SimpleParameters,
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
			State_CollHdrComputedParameters,
			State_CollHdrReactions,
			State_CollHdrSimpleParameters,
			State_CollHdrSpecies,

			State_Count
		};

		/*!
		@brief Pointer to the sbml module represented by this node.
		*/
		ECellEngine::Data::SBMLModule* data;

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
		unsigned char utilityState = 0;

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

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		AssetNodeData(ECellEngine::Data::Module* _data) :
			NodeData(), data{ dynamic_cast<ECellEngine::Data::SBMLModule*>(_data) }
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			inputPins[InputPin_Solver] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Solver, this);//Solver

			outputPins[OutputPin_CollHdrComputedParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Species Collapsing header
			outputPins[OutputPin_CollHdrReactions] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Simple Parameters Collapsing header
			outputPins[OutputPin_CollHdrSimpleParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters Collapsing header
			outputPins[OutputPin_CollHdrSpecies] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Reaction, this);//Reactions Collapsing header

			collapsingHeadersIds[CollapsingHeader_ComputedParameters] = Widget::MNBV::GetMNBVCtxtNextId();//Species Collapsing header
			collapsingHeadersIds[CollapsingHeader_Reactions] = Widget::MNBV::GetMNBVCtxtNextId();//Simple Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_SimpleParameters] = Widget::MNBV::GetMNBVCtxtNextId();//Computed Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_Species] = Widget::MNBV::GetMNBVCtxtNextId();//Reactions Collapsing header

			//Initialize the list boxes data
			nlbsData[NodeListBoxString_ComputedParameters] = { &data->GetAllComputedParameter(), Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_Reactions] = { &data->GetAllReaction(), Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_SimpleParameters] = { &data->GetAllSimpleParameter(), Widget::MNBV::GetMNBVCtxtNextId() };
			nlbsData[NodeListBoxString_Species] = { &data->GetAllSpecies(), Widget::MNBV::GetMNBVCtxtNextId() };

		}

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in asset node data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override;

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in asset node data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in asset node data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override {};//not used in asset node data

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::ComputedParameter.
	*/
	struct ComputedParameterNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
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

		/*!
		@brief The local enum to manage access to the output pins.
		@see ::outputPins
		*/
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

		/*!
		@brief The local enum to manage access to the collapsing headers.
		@see ::collapsingHeaders
		*/
		enum CollapsingHeader
		{
			CollapsingHeader_ModelLinks,
			CollapsingHeader_ComputedParameters,
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
			NodeListBoxString_ComputedParameterOperands,
			NodeListBoxString_SimpleParameterOperands,
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
			State_CollHdrComputedParameters,
			State_CollHdrKineticLaws,
			State_CollHdrDataFields,
			State_CollHdrEquationOperands,

			State_Count
		};

		/*!
		@brief Pointer to the computed parameter represented by this node.
		*/
		std::shared_ptr<ECellEngine::Data::ComputedParameter> data;

		const ECellEngine::Data::DependenciesDatabase& depDB;

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
		unsigned char utilityState = 0;

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
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::ComputedParameter>> nlbsDataCPDep;
		NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::Reaction>> nlbsDataRDep;
		std::vector<std::weak_ptr<ECellEngine::Data::ComputedParameter>> computedParameterDep;
		std::vector<std::weak_ptr<ECellEngine::Data::Reaction>> reactionDep;
		std::vector<std::string> speciesOperands;
		std::vector<std::string> simpleParametersOperands;
		std::vector<std::string> computedParametersOperands;

		ComputedParameterNodeData(const ComputedParameterNodeData& _cpnd) :
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
			nlbsDataCPDep{ _cpnd.nlbsDataCPDep }, nlbsDataRDep { _cpnd.nlbsDataRDep },
			computedParameterDep{ _cpnd.computedParameterDep },
			reactionDep{ _cpnd.reactionDep },
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

			nlbsDataCPDep.data = &computedParameterDep;
			nlbsDataRDep.data = &reactionDep;
			nlbsData[NodeListBoxString_SpeciesOperands].data = &speciesOperands;
			nlbsData[NodeListBoxString_SimpleParameterOperands].data = &simpleParametersOperands;
			nlbsData[NodeListBoxString_ComputedParameterOperands].data = &computedParametersOperands;
		}

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		ComputedParameterNodeData(std::shared_ptr<ECellEngine::Data::ComputedParameter> _data, const ECellEngine::Data::DependenciesDatabase& _depDB) :
			NodeData(), data{ _data }, depDB{ _depDB }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Asset, this);//Asset
			inputPins[InputPin_CollHdrComputedParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			inputPins[InputPin_CollHdrKineticLaws] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			inputPins[InputPin_CollHdrDataFields] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			inputPins[InputPin_CollHdrEquationOperands] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Equation Operands collapsing header
			inputPins[InputPin_NLBSSpecies] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Node String List Box for Species Operands
			inputPins[InputPin_NLBSSimpleParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Simple Parameter Operands
			inputPins[InputPin_NLBSComputedParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Computed Parameter Operands
			inputPins[InputPin_ComputedParameterValue] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Operation Value Float

			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			outputPins[OutputPin_CollHdrComputedParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			outputPins[OutputPin_CollHdrKineticLaws] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			outputPins[OutputPin_CollHdrDataFields] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			outputPins[OutputPin_CollHdrEquationOperands] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Equation Operands collapsing header
			outputPins[OutputPin_NLBSSpecies] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Node String List Box for Species Operands
			outputPins[OutputPin_NLBSSimpleParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Simple Parameter Operands
			outputPins[OutputPin_NLBSComputedParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Node String List Box for Computed Parameter Operands
			outputPins[OutputPin_ComputedParameterValue] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Operation Value Float

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//ModelLinks Collapsing header
			collapsingHeadersIds[CollapsingHeader_ComputedParameters] = Widget::MNBV::GetMNBVCtxtNextId();//Computed Parameters section
			collapsingHeadersIds[CollapsingHeader_KineticLaws] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Laws section
			collapsingHeadersIds[CollapsingHeader_DataFields] = Widget::MNBV::GetMNBVCtxtNextId();//Data Fields collapsing header
			collapsingHeadersIds[CollapsingHeader_EquationOperands] = Widget::MNBV::GetMNBVCtxtNextId();//Equation Operands collapsing header

			computedParameterDep = depDB.GetComputedParameterDependencies().find(data)->second.partOfComputedParameter;
			nlbsDataCPDep = { &computedParameterDep , Widget::MNBV::GetMNBVCtxtNextId() };
			reactionDep = depDB.GetComputedParameterDependencies().find(data)->second.partOfReactionKineticLaw;
			nlbsDataRDep = { &reactionDep , Widget::MNBV::GetMNBVCtxtNextId() };
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);//Node String List Box for Species Operands
			nlbsData[NodeListBoxString_SpeciesOperands] = { &speciesOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Node String List Box for Species Operands
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);//Node String List Box for Simple Parameter Operands
			nlbsData[NodeListBoxString_SimpleParameterOperands] = { &simpleParametersOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Node String List Box for Simple Parameter Operands
			_data->GetOperation().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);//Node String List Box for Computed Parameter Operands
			nlbsData[NodeListBoxString_ComputedParameterOperands] = { &computedParametersOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Node String List Box for Computed Parameter Operands
		}

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in computed parameter node data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in computed parameter node data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in computed parameter node data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node to set up and
			display a line plot.
	*/
	struct LinePlotNodeData final : public NodeData
	{
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
			State_CollHdrPlot,

			State_Count
		};

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
		@brief The byte the encode the state variations of this node.
		@details Manipulate the state with the enum values State_XXX
		*/
		unsigned char utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

		LinePlotNodeData(int _maxNbDataPoints, ImVec2& _position) :
			NodeData(), dataPoints{ _maxNbDataPoints }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_CollHdrPlot] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Plot Collapsing Header
			inputPins[InputPin_XAxis] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//X
			inputPins[InputPin_YAxis] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Y

			outputPins[OutputPin_None] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this); //not used

			collapsingHeadersIds[CollapsingHeader_AllParameters] = Widget::MNBV::GetMNBVCtxtNextId();//Parameters Collapsing header
			collapsingHeadersIds[CollapsingHeader_GeneralParameters] = Widget::MNBV::GetMNBVCtxtNextId();//General (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_PlotFlags] = Widget::MNBV::GetMNBVCtxtNextId();//Plot Flags (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_XAxisFlags] = Widget::MNBV::GetMNBVCtxtNextId();//X Axis Flags (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_YAxisFlags] = Widget::MNBV::GetMNBVCtxtNextId();//Y Axis Falgs (Parameters) Collapsing header
			collapsingHeadersIds[CollapsingHeader_Plot] = Widget::MNBV::GetMNBVCtxtNextId();//Plot Collapsing Header

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
			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}
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
			InputPin_NLBSComputedParameters,
			InputPin_NLBSSimpleParameters,
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
			OutputPin_NLBSComputedParameters,
			OutputPin_NLBSSimpleParameters,
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
			NodeListBoxString_ComputedParameterOperands,
			NodeListBoxString_SimpleParameterOperands,
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
		unsigned char utilityState = 0;

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
			nlbsData[NodeListBoxString_SpeciesOperands].data = &speciesOperands;
			nlbsData[NodeListBoxString_SimpleParameterOperands].data = &simpleParametersOperands;
			nlbsData[NodeListBoxString_ComputedParameterOperands].data = &computedParametersOperands;

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
		ReactionNodeData(ECellEngine::Data::Reaction* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Reaction, this);//Asset
			inputPins[InputPin_CollHdrReactants] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactants section Collapsing header
			inputPins[InputPin_CollHdrProducts] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Products section Collapsing header
			inputPins[InputPin_CollHdrKineticLaw] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Collapsing header
			inputPins[InputPin_CollHdrKineticLawOperands] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Operands Collapsing header
			inputPins[InputPin_NLBSSpecies] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Species Operands from Kinetic Law
			inputPins[InputPin_NLBSSimpleParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Simple Parameter Operands from Kinetic Law
			inputPins[InputPin_NLBSComputedParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameter Operands from Kinetic Law
			inputPins[InputPin_KineticLawValue] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Kinetic Law Value Float field (must be Read Only)

			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			outputPins[OutputPin_CollHdrReactants] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactants section Collapsing header
			outputPins[OutputPin_CollHdrProducts] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Products section Collapsing header
			outputPins[OutputPin_CollHdrKineticLaw] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Collapsing header
			outputPins[OutputPin_CollHdrKineticLawOperands] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Kinetic Law Operands Collapsing header
			outputPins[OutputPin_NLBSSpecies] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Species Operands from Kinetic Law
			outputPins[OutputPin_NLBSSimpleParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Simple Parameter Operands from Kinetic Law
			outputPins[OutputPin_NLBSComputedParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameter Operands from Kinetic Law
			outputPins[OutputPin_KineticLawValue] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Kinetic Law Value Float field (must be Read Only)

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//ModelLinks Collapsing header
			collapsingHeadersIds[CollapsingHeader_Reactants] = Widget::MNBV::GetMNBVCtxtNextId();//Reactants section Collapsing header
			collapsingHeadersIds[CollapsingHeader_Products] = Widget::MNBV::GetMNBVCtxtNextId();//Products section Collapsing header
			collapsingHeadersIds[CollapsingHeader_KineticLaw] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Law collapsing header
			collapsingHeadersIds[CollapsingHeader_KineticLawOperands] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Law Operands collapsing header

			nlbsData[NodeListBoxString_Reactants] = { _data->GetReactants() , Widget::MNBV::GetMNBVCtxtNextId() };//Reactants section
			nlbsData[NodeListBoxString_Products] = { data->GetProducts(), Widget::MNBV::GetMNBVCtxtNextId() };//Products section
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::Species>(speciesOperands);
			nlbsData[NodeListBoxString_SpeciesOperands] = { &speciesOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Species Operands from Kinetic Law
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::SimpleParameter>(simpleParametersOperands);
			nlbsData[NodeListBoxString_SimpleParameterOperands] = { &simpleParametersOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Simple Parameter Operands from Kinetic Law
			_data->GetKineticLaw().GetOperandsNames<ECellEngine::Data::ComputedParameter>(computedParametersOperands);
			nlbsData[NodeListBoxString_ComputedParameterOperands] = { &computedParametersOperands, Widget::MNBV::GetMNBVCtxtNextId() };//Computed Parameter Operands from Kinetic Law
		}

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Reaction Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Reaction Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Reaction Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::SimpleParameter.
	*/
	struct SimpleParameterNodeData final : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_CollHdrModelLinks,
			InputPin_CollHdrComputedParameters,
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
			OutputPin_CollHdrModelLinks,
			OutputPin_CollHdrComputedParameters,
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
			CollapsingHeader_ComputedParameters,
			CollapsingHeader_KineticLaws,
			CollapsingHeader_DataFields,

			CollapsingHeader_Count
		};

		/*!
		@brief The local enum to manage access to the list boxes storing strings.
		@see ::nlbsData
		*/
		enum NodeListBoxString
		{
			NodeListBoxString_ComputedParameterLinks,
			NodeListBoxString_KineticLaws,

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
			State_CollHdrComputedParameters,
			State_CollHdrKineticLaws,
			State_CollHdrDataFields,

			State_Count
		};

		/*!
		@brief Pointer to the simple parameter represented by this node.
		*/
		ECellEngine::Data::SimpleParameter* data;

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
		unsigned char utilityState = 0;

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
		std::vector<std::string> computedParameterLinks;
		std::vector<std::string> reactionLinks;

		SimpleParameterNodeData(const SimpleParameterNodeData& _sprnd) :
			NodeData(_sprnd), data{ _sprnd.data },
			inputPins{ _sprnd.inputPins[0], _sprnd.inputPins[1] , _sprnd.inputPins[2] ,
					  _sprnd.inputPins[3] , _sprnd.inputPins[4] , _sprnd.inputPins[5] },
			outputPins{ _sprnd.outputPins[0], _sprnd.outputPins[1] , _sprnd.outputPins[2] ,
					  _sprnd.outputPins[3] , _sprnd.outputPins[4] },
			utilityState{ _sprnd.utilityState },
			collapsingHeadersIds{ _sprnd.collapsingHeadersIds[0], _sprnd.collapsingHeadersIds[1] , _sprnd.collapsingHeadersIds[2] ,
					  _sprnd.collapsingHeadersIds[3] },
			nlbsData{ _sprnd.nlbsData[0], _sprnd.nlbsData[1] },
			computedParameterLinks{ _sprnd.computedParameterLinks }, reactionLinks{ _sprnd.reactionLinks }
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
		SimpleParameterNodeData(ECellEngine::Data::SimpleParameter* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));


			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Asset
			inputPins[InputPin_CollHdrComputedParameters] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			inputPins[InputPin_CollHdrKineticLaws] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			inputPins[InputPin_CollHdrDataFields] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			inputPins[InputPin_ParameterValue] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Value Float field

			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//ModelLinks Collapsing header
			outputPins[OutputPin_CollHdrComputedParameters] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Computed Parameters section
			outputPins[OutputPin_CollHdrKineticLaws] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Parameter, this);//Kinetic Laws section
			outputPins[OutputPin_CollHdrDataFields] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Data Fields collapsing header
			outputPins[OutputPin_ParameterValue] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Value Float field

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//ModelLinks Collapsing header
			collapsingHeadersIds[CollapsingHeader_ComputedParameters] = Widget::MNBV::GetMNBVCtxtNextId();//Computed Parameters section
			collapsingHeadersIds[CollapsingHeader_KineticLaws] = Widget::MNBV::GetMNBVCtxtNextId();//Kinetic Laws section
			collapsingHeadersIds[CollapsingHeader_DataFields] = Widget::MNBV::GetMNBVCtxtNextId();//Data Fields collapsing header

			nlbsData[NodeListBoxString_ComputedParameterLinks] = { &computedParameterLinks, Widget::MNBV::GetMNBVCtxtNextId() }; //Computed Parameters section
			nlbsData[NodeListBoxString_KineticLaws] = { &reactionLinks, Widget::MNBV::GetMNBVCtxtNextId() };//Kinetic Laws section
		}

		void InputConnect(const NodeInputPinData& _nodeInput) override;

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Reaction Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override;

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;

		/*!
		@brief Resets the state of the node list box string data of this node.
		@see ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData::ResetUtilityState()
		*/
		void ResetNLBSDUtilityStates() noexcept;
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
		@brief A buffer for the elapsed time value in the ::simulationTimer to
				detect when it has been changed.
		*/
		float elapsedTimeBuffer = 0.f;

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
			outputPins[OutputPin_SimulationTime] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this); //simulation Time
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

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Simulation Time Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Simulation Time Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Simulation Time Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in Simulation Time Node Data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::Solver.
	*/
	struct SolverNodeData : public NodeData
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
		ECellEngine::Solvers::Solver* data;

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

		/*!
		@remarks @p _nodeId is incremented immediately after use.
		*/
		SolverNodeData(ECellEngine::Solvers::Solver* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImGui::GetIO().MousePos);

			inputPins[InputPin_None] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//not used
			outputPins[OutputPin_Solver] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Solver, this);//this solver transmission

		}

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Solver Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Solver Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Solver Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in Solver Node Data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
	};

	/*!
	@brief The logic to encode the data needed to draw the node representing
			ECellEngine::Data::Species.
	*/
	struct SpeciesNodeData : public NodeData
	{
		/*!
		@brief The local enum to manage access to the input pins.
		@see ::inputPins
		*/
		enum InputPin
		{
			InputPin_CollHdrModelLinks,
			InputPin_CollHdrDataFields,
			InputPin_NLBSAsReactant,
			InputPin_NLBSAsProduct,
			InputPin_NLBSInComputedParameter,
			InputPin_NLBSInKineticLaw,
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
			OutputPin_CollHdrModelLinks,
			OutputPin_CollHdrDataFields,
			OutputPin_NLBSAsReactant,
			OutputPin_NLBSAsProduct,
			OutputPin_NLBSInComputedParameter,
			OutputPin_NLBSInKineticLaw,
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
			State_CollHdrDataFields,

			State_Count
		};

		/*!
		@brief Pointer to the species represented by this node.
		*/
		ECellEngine::Data::Species* data;

		/*!
		@brief A buffer for the quantity value of a species to detect when the
				value in the engine has been changed.
		*/
		float speciesQuantityBuffer = 0.f;

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
		unsigned char utilityState = 0;

		/*!
		@brief All the collapsing headers.
		@details Access the pins with the enum values CollapsingHeader_XXX
		*/
		std::size_t collapsingHeadersIds[CollapsingHeader_Count];

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
			for (int i = 0; i < InputPin_Count; i++)
			{
				inputPins[i].node = this;
			}

			for (int i = 0; i < OutputPin_Count; i++)
			{
				outputPins[i].node = this;
			}
		}

		SpeciesNodeData(ECellEngine::Data::Species* _data) :
			NodeData(), data{ _data }
		{
			ax::NodeEditor::SetNodePosition(id, ImVec2(300.f + ImGui::GetIO().MousePos.x, 0.f + ImGui::GetIO().MousePos.y));

			inputPins[InputPin_CollHdrModelLinks] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Model Links
			inputPins[InputPin_Asset] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Asset
			inputPins[InputPin_NLBSInComputedParameter] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Computed parameters equation
			inputPins[InputPin_NLBSAsReactant] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactions's Reactants
			inputPins[InputPin_NLBSAsProduct] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Products
			inputPins[InputPin_NLBSInKineticLaw] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Kinetic Law
			inputPins[InputPin_CollHdrDataFields] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Data Fields
			inputPins[InputPin_Quantity] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Quantity

			outputPins[OutputPin_CollHdrModelLinks] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Model Links
			outputPins[OutputPin_NLBSInComputedParameter] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Computed parameters equation
			outputPins[OutputPin_NLBSAsReactant] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reactions's Reactants
			outputPins[OutputPin_NLBSAsProduct] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Products
			outputPins[OutputPin_NLBSInKineticLaw] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Species, this);//Reaction's Kinetic Law
			outputPins[OutputPin_CollHdrDataFields] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this);//Collapsing Header Data Fields
			outputPins[OutputPin_Quantity] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this);//Quantity

			collapsingHeadersIds[CollapsingHeader_ModelLinks] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Model Links
			collapsingHeadersIds[CollapsingHeader_DataFields] = Widget::MNBV::GetMNBVCtxtNextId();//Collapsing Header Data Fields
		}

		void InputConnect(const NodeInputPinData& _nodeInput) override;

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Species Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override;

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override;

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
	};

	/*!
	@brief The logic to encode the data needed to draw the node to define and
			use a custom float.
	*/
	struct ValueFloatNodeData : public NodeData
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
		@brief The value to make accessible through this node.
		*/
		float value = 0.f;

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
			NodeData(), value{ _value }
		{
			ax::NodeEditor::SetNodePosition(id, _position);

			inputPins[InputPin_None] = NodeInputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_Default, this); //not used
			outputPins[OutputPin_Value] = NodeOutputPinData(Widget::MNBV::GetMNBVCtxtNextId(), PinType_ValueFloat, this); //Value
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

		void InputConnect(const NodeInputPinData& _nodeInput) override {};//not used in Value Float Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, char* _data) override {};//not used in Value Float Node Data

		void InputUpdate(const NodeInputPinData& _nodeInput, float _data) override {};//not used in Value Float Node Data

		void OutputConnect(const NodeOutputPinData& _nodeOutput) override {};//not used in Value Float Node Data

		void OutputUpdate(const NodeOutputPinData& _nodeOutput) override;
	};
#pragma endregion
}