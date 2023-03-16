#pragma once
#include "imgui_node_editor.h"
#include "Logger.hpp"

namespace ECellEngine::Editor::Utility
{
	/*!
	@brief Struct to hold basic information about connection between pins.
	@details Note that connection (aka. link) has its own ID. This is useful
			 later with dealing with selections, deletion or other operations.
	@remarks Originally from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct LinkInfo
	{
		ax::NodeEditor::LinkId Id;
		ax::NodeEditor::PinId  InputId;
		ax::NodeEditor::PinId  OutputId;
	};

	/*!
	@brief Struct with public static logic to help building the NodeEditor.
	@remarks Contains code from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct NodeEditorUtility
	{
		 
		/*!
		@brief To start a column in a node.
		@details Effectively is ImGui::BeginGroup().
		@see ::NextColumn
		@see ::EndColumn
		*/
		inline static void BeginColumn()
		{
			ImGui::BeginGroup();
		}

		/*!
		@brief Draws a simple node.
		@details The number of input and output pins can be cutomized. The
				 pins are drawn using ::DrawInputPin and ::DrawOutputPin.
		@param _idx The index of this node to draw. The value is incremented
					immediately after it is used for this node.
		@param _name The name of the node to dislay.
		@param _nbInputPin The number of input pins to add to the node.
		@param _inputPinNames The array of the names of each input pin
				to draw.
		@param _nbOututPin The number of input pins to add to the node.
		@param _outputPinNames The array of the names of each output pin
				to draw.
		@remarks The number of strings encoded in @p _inputPinNames MUST
				 be identical to @p _nbInputPin. If @p _inputPinNames <
				 @p _nbInputPin, an index error will be generated. If
				 If @p _inputPinNames > @p _nbInputPin no errors will be 
				 generated but not every pins will be drawn. It is the
				 same behavior for (@p _outputPinNames , @p _nbOutputPin).
		*/
		static void DrawNode(
			int& _idx, const char* _name,
			const short _nbInputPin, const char* _inputPinNames[],
			const short _nbOutputPin, const char* _outputPinNames[]);

		/*!
		@brief Draw a basic input pin symbolized by an arrow. 
		@param _idx The index of this pin to draw. The value is incremented
					immediately after it is used for this pin.
		@param _name The name of the pin to dislay after the arrow.
		*/
		static void DrawInputPin(int& _idx, const char* _name);

		/*!
		@brief Draw a basic output pin symbolized by an arrow.
		@param _idx The index of this pin to draw. The value is incremented
					immediately after it is used for this pin.
		@param _name The name of the pin to dislay before the arrow.
		*/
		static void DrawOutputPin(int& _idx, const char* _name);

		/*!
		@brief The basic logic to create links between input and output
				pins of nodes.
		@param _id The current value of the ID counter for links.
		@param _links The current list of links.
		*/
		static void LinkCreation(int& _id, std::vector<ECellEngine::Editor::Utility::LinkInfo>& _links);

		/*!
		@brief The basic logic to destroy links between nodes.
		@param _links The current list of links.
		*/
		static void LinkDestruction(std::vector<ECellEngine::Editor::Utility::LinkInfo>& _links);

		/*!
		@brief To start a new column after one has already been opened.
		@details Effectively closes the group started in ::BeginColumn and
				 starts a new one.
		@see ::BeginColumn
		@see ::EndColumn
		*/
		inline static void NextColumn()
		{
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
		}

		/*!
		@brief To close a column in a node.
		@details Effectively is ImGui::EndGroup().
		@see ::BeginColumn
		@see ::NextColumn
		*/
		inline static void EndColumn()
		{
			ImGui::EndGroup();
		}


	};
}