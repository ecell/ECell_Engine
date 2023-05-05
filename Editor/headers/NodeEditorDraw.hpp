#pragma once

#include "imgui_internal.h"
#include "NodeEditorData.hpp" //also include "implot.h"

#include "Logger.hpp"

namespace ECellEngine::Editor::Utility
{
	/*!
	@brief Struct with public static logic to help building the NodeEditor.
	@remarks Contains code from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct NodeEditorDraw
	{
#pragma region Custom Styles

		/*!
		@brief Pops the number of color styles pushed with ::PushNodeStyle(const ImVec4 _colorSet[])
		*/
		inline static void PopNodeStyle()
		{
			ImGui::PopStyleColor(4);
			ax::NodeEditor::PopStyleColor(2);
		}

		/*!
		@brief Pushes a bunch of colors to customize a node' style.
		@details Don't forget to call ::PopNodeStyle() after you've drawn the
				 node.
		@see ::PopNodeStyle()
		*/
		inline static void PushNodeStyle(const ImVec4 _colorSet[])
		{
			ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, _colorSet[NodeColorType_Bg]);
			ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, _colorSet[NodeColorType_Border]);
			ImGui::PushStyleColor(ImGuiCol_Button, _colorSet[NodeColorType_Bg]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, _colorSet[NodeColorType_HeaderActivated]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _colorSet[NodeColorType_HeaderHovered]);
			ImGui::PushStyleColor(ImGuiCol_Separator, _colorSet[NodeColorType_Border]);
		}

		/*!
		@brief Pops the number of styles pushed with ::PushScrollBarStyle(ImGuiStyle& _style)
		*/
		inline static void PopScrollBarStyle()
		{
			ImGui::PopStyleColor(4);
			ImGui::PopStyleVar(2);
		}

		/*!
		@brief Pushes a bunch of styles to match ImGui's default Scroll Bar style.
				Can be used with sliders in nodes.
		@details Don't forget to call ::PopScrollBarStyle() after you've drawn the
				 slider.
		@see ::PopScrollBarStyle()
		*/
		inline static void PushScrollBarStyle(const ImGuiStyle& _style)
		{
			//Style the vertical slider like a scroll bar
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, _style.ScrollbarRounding);
			ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, _style.ScrollbarRounding);
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
		}
#pragma endregion

#pragma region Nodes
		/*!
		@brief Draws a node to display and access the content of an
				ECellEngine::Data::Module.
		@param _name The name of the node. It will appear in the header of the
				node in the editor.
		@param _assetNodeInfo The struct with information about what to draw.
		@param _assetNodeColors The set of colors to cutomize the looks of this
				node.
		@param _solverPinColors The set of colors to cutomize the solver pin in
				this node.
		@param _parameterPinColors The set of colors to cutomize the parameter
				pin in this node.
		@param _reactionPinColors The set of colors to cutomize the reaction pin
				in this node.
		@param _speciesPinColors The set of colors to cutomize the species pin in
				this node.
		*/
		static void AssetNode(const char* _name, AssetNodeData& _assetNodeInfo);

		static void ComputedParameterNode(const char* _name, ComputedParameterNodeData& _parameterNodeInfo);
		
		static void ReactionNode(const char* _name, ReactionNodeData& _reactionNodeInfo);

		static void SimpleParameterNode(const char* _name, SimpleParameterNodeData& _parameterNodeInfo);
		/*!
		@brief Draws a node to display and access the content of a
				ECellEngine::Solvers::Solver.
		@param _name The name of the node. It will appear in the header of the
				node in the editor.
		@param _solverNodeInfo The struct with information about what to draw.
		@param _solverNodeColors The set of colors to cutomize the looks of this
				node.
		@param _assetPinColors The set of colors to cutomize the asset pin in
				this node.
		*/
		static void SolverNode(const char* _name, const SolverNodeData& _solverNodeInfo);

		static void SpeciesNode(const char* _name, SpeciesNodeData& _speciesNodeInfo);

#pragma endregion

#pragma region Node Pins
		
		/*!
		@brief Draws a Link in the node Editor.
		@details Draws the first successful link according to the order (i.e.
				 priority) of ids in ECellEngine::Editor::Utility::LinkData::startIds
				 and ECellEngine::Editor::Utility::LinkData::endIds.
		*/
		static void Link(LinkData& linkInfo);

		/*!
		@brief The basic logic to create links between input and output
				pins of nodes.
		@param _links The current list of links.
		*/
		static void LinkCreation(std::vector<ECellEngine::Editor::Utility::LinkData>& _links);

		/*!
		@brief The basic logic to destroy links between nodes.
		@param _links The current list of links.
		*/
		static void LinkDestruction(std::vector<ECellEngine::Editor::Utility::LinkData>& _links);

		/*!
		@brief Draw a square pin.
		@param _pinData The struct containing the information about the
				pin to be drawn.
		@param _pinColors The set of colors to cutomize the looks of this pin.
		*/
		static void Pin(const NodePinData& _pinData, const ImVec4 _pinColors[]);
#pragma endregion

#pragma region Layout

		/*!
		@brief Offsets the current position of the cursor (the drawer in ImGui) by
				the width of a Node pin and the default Item Spacing on the X Axis.
		@details This is particularly usefull when we need to align the center
				 components but there was no need to draw an input pin on the left
				 side.
		@todo Currently, the size of the pin itself is hardcoded to the default
			  value (8 pixels). This must be updated once the size will be set
			  globally in the Node Style struct.
		*/
		inline static void ApplyPinDrawOffset()
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + GetMNBVStyle()->pinWidth + ImGui::GetStyle().ItemSpacing.x);
		}

		/*!
		@brief Offsets the current position of the cursor (the drawer in ImGui)
				so that a set of item to be drawn will be right-aligned at
				@p _targetLength.
		@param _startX The start position on the X axis for the alignment
				calculations.
		@param _targetLength The length at which we want to align some items on
				the right.
		@param _itemWidths The widths of the set of items we want to align on
				the right.
		@param _nbItems The number of items to align.
		*/
		inline static void AlignAllToRight(const float _startX, const float _targetLength, const float _itemWidths[], const std::size_t _nbItems)
		{
			float offset = 0;
			for (std::size_t i = 0; i < _nbItems; i++)
			{
				offset += _itemWidths[i];
			}
			ImGui::SetCursorPosX(_startX + _targetLength - offset);
		}

		/*!
		@brief Offsets the current position of the cursor (the drawer in ImGui)
				so that the next item will be center-aligned on @p _targetLength.
		@param _startX The start position on the X axis for the alignment
				calculations.
		@param _targetLength The length at which we want to align some items at
				the center.
		@param _itemWidth The width of the item we want to align at the center.
		*/
		inline static void AlignToCenter(const float _startX, const float _targetLength, const float _itemWidth)
		{
			ImGui::SetCursorPosX(_startX + 0.5f * (_targetLength - _itemWidth));
		}

		/*!
		@brief Offsets the current position of the cursor (the drawer in ImGui)
				so that the next item will be right-aligned at @p _targetLength.
		@param _startX The start position on the X axis for the alignment
				calculations.
		@param _targetLength The length at which we want to align some items on
				the right.
		@param _itemWidth The width of the item we want to align on the right.
		*/
		inline static void AlignToRight(const float _startX, const float _targetLength, const float _itemWidth)
		{
			ImGui::SetCursorPosX(_startX + _targetLength - _itemWidth);
		}

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
		@brief To close a column in a node.
		@details Effectively is ImGui::EndGroup().
		@see ::BeginColumn
		@see ::NextColumn
		*/
		inline static void EndColumn()
		{
			ImGui::EndGroup();
		}

		/*!
		@brief Computes the offset generated by drawing a Node pin and the
				Item Spacing on the X Axis that we put after that.
		@todo Currently, the size of the pin itself is hardcoded to the default value (8 pixels).
			  This must be updated once the size will be set globally in the Node Style struct.
		*/
		inline static float GetPinDrawOffset()
		{
			return GetMNBVStyle()->pinWidth + ImGui::GetStyle().ItemSpacing.x;
		}

		inline static float GetNodeCenterAreaWidth(const float _headerWidth, const short _nbPins = 2)
		{
			return std::max(GetMNBVStyle()->nodeCenterAreaMinWidth, _headerWidth - _nbPins * GetMNBVStyle()->pinWidth - _nbPins * ImGui::GetStyle().ItemSpacing.x);
		}

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
#pragma endregion

#pragma region Custom Node Widget
		/*!
		@brief Custom collapsing header for nodes with no pins.
		@details The collapsing header is a button that changes the value of the
				@p utilityState. The button is drawn at the center of the node.
		@remarks This intends to reproduce ImGui::CollapsingHeader but without
				 relying on ImGui's tree nodes API. Because it is not possible
				 (as of ImGui 1.89) to control the width of a tree node which
				 creates a visual artifact: the node extends infinitely because
				 in the Node Editor virtual area has not limits.
		@param _label The text to display in the collapsing header.
		@param _id Unique integer ID to identify the collapsing header. This is
				mandatory to not mess up ImGui's internal identification system.
		@param _utilityState The reference to the character containing the encoding
				of the the open/close state of this header.
		@param _stateBitPos The position of the bit in @p _utilityState that
				encodes the open/close state of this header
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _size The size of the button representing the collapsing header.
		*/
		static bool NodeCollapsingHeader(const char* _label, const std::size_t _id,
			unsigned char& _utilityState, const short _stateBitPos,
			const float _startX, const float _drawLength,
			const ImVec2& _size = ImVec2(0, 0));

		/*!
		@brief Custom collapsing header for nodes with an input pin.
		@details The collapsing header is a button that changes the value of the
				@p utilityState. The button is drawn at the center of the node.
		@remarks This intends to reproduce ImGui::CollapsingHeader but without
				 relying on ImGui's tree nodes API. Because it is not possible
				 (as of ImGui 1.89) to control the width of a tree node which
				 creates a visual artifact: the node extends infinitely because
				 in the Node Editor virtual area has not limits.
		@param _label The text to display in the collapsing header.
		@param _id Unique integer ID to identify the collapsing header. This is
				mandatory to not mess up ImGui's internal identification system.
		@param _utilityState The reference to the character containing the encoding
				of the the open/close state of this header.
		@param _stateBitPos The position of the bit in @p _utilityState that
				encodes the open/close state of this header
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _pin The pin data used for the output pin to draw.
		@param _pinColors The set of colors to cutomize the input pins.
		@param _size The size of the button representing the collapsing header.
		@param _hidePinsOnExpand A boolean to control whether the input pin associated
				to the collapsing header shall NOT be drawn when the collapsing header
				is open (displaying the content bellowit).
		*/
		static bool NodeCollapsingHeader_In(const char* _label, const std::size_t _id,
			unsigned char& _utilityState, const short _stateBitPos,
			const float _startX, const float _drawLength,
			const NodePinData& _pin, const ImVec4 _pinColors[],
			const ImVec2& _size = ImVec2(0, 0), const bool _hidePinsOnExpand = true);
		
		/*!
		@brief Custom collapsing header for nodes with an input pin and output
				pin.
		@details The collapsing header is a button that changes the value of the
				@p utilityState. The button is drawn at the center of the node.
		@remarks This intends to reproduce ImGui::CollapsingHeader but without
				 relying on ImGui's tree nodes API. Because it is not possible
				 (as of ImGui 1.89) to control the width of a tree node which
				 creates a visual artifact: the node extends infinitely because
				 in the Node Editor virtual area has not limits.
		@param _label The text to display in the collapsing header.
		@param _id Unique integer ID to identify the collapsing header. This is
				mandatory to not mess up ImGui's internal identification system.
		@param _utilityState The reference to the character containing the encoding
				of the the open/close state of this header.
		@param _stateBitPos The position of the bit in @p _utilityState that
				encodes the open/close state of this header
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _inputPin The pin data used for the input pin to draw.
		@param _outputPin The pin data used for the output pin to draw.
		@param _pinColors The set of colors to cutomize both pins.
		@param _size The size of the button representing the collapsing header.
		@param _hidePinsOnExpand A boolean to control whether the pins associated to
				the collapsing header shall NOT be drawn when the collapsing header
				is open (displaying the content bellowit).
		*/
		static bool NodeCollapsingHeader_InOut(const char* _label, const std::size_t _id,
			unsigned char& _utilityState, const short _stateBitPos,
			const float _startX, const float _drawLength,
			const NodePinData& _inputPin, const NodePinData& _outputPin, const ImVec4 _pinColors[],
			const ImVec2& _size = ImVec2(0, 0), const bool _hidePinsOnExpand = true);
			
		/*!
		@brief Custom collapsing header for nodes with an output pin.
		@details The collapsing header is a button that changes the value of the
				@p utilityState. The button is drawn at the center of the node
				and the side of the button extends until it touches the output
				pin.
		@remarks This intends to reproduce ImGui::CollapsingHeader but without
				 relying on ImGui's tree nodes API. Because it is not possible
				 (as of ImGui 1.89) to control the width of a tree node which
				 creates a visual artifact: the node extends infinitely because
				 in the Node Editor virtual area has not limits.
		@param _label The text to display in the collapsing header.
		@param _id Unique integer ID to identify the collapsing header. This is
				mandatory to not mess up ImGui's internal identification system.
		@param _utilityState The reference to the character containing the encoding
				of the the open/close state of this header.
		@param _stateBitPos The position of the bit in @p _utilityState that
				encodes the open/close state of this header
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _pin The pin data used for the output pin to draw.
		@param _pinColors The set of colors to cutomize the output pins.
		@param _size The size of the button representing the collapsing header.
		@param _hidePinsOnExpand A boolean to control whether the output pin associated
				to the collapsing header shall NOT be drawn when the collapsing header
				is open (displaying the content bellowit).
		*/
		static bool NodeCollapsingHeader_Out(const char* _label, const std::size_t _id,
			unsigned char& _utilityState, const short _stateBitPos,
			const float _startX, const float _drawLength,
			const NodePinData& _pin, const ImVec4 _pinColors[],
			const ImVec2& _size = ImVec2(0, 0), const bool _hidePinsOnExpand = true);

		/*!
		@brief Logic to draw headers of nodes.
		@param _type A string that describes the type of the node.
		@param _name The name of the node. It is displayed right after @p _type.
		@param _colorSet The array of colors to style the header similarly to the
				rest of the node.
		@param _width The minimal width of the header. The header may be bigger than
				this width if the string @p _type + @p _name is bigger.
		@param _height The height of the header. It is counted in number of lines.
		@param _nbPins Number of sides of the node that has pins. Enter 0 if the
				node has no pins on the input nor output side. Enter 1 if the node
				has pins on the input side or the output side. Enter 2 if the node
				has pins on the input side and ouput side.
		@returns The final width of the header.
		*/
		static float NodeHeader(const char* _type, const char* _name, const ImVec4 _colorSet[],
			const float _width = 200.f, const short _height = 1, const short _nbPins = 2);

		/*!
		@brief Draws a horizontal line of a set width and thickness.
		@details Intends to replace ImGui::Separator() which spans infinitely in width within
				 the node editor.
		@param _width The length of the line to draw as a seperator.
		@param _thickness The thickness in pixel of the line to draw as a separator.
		*/
		static void NodeHorizontalSeparator(const float _width, const float _thickness = 1.f);

		/*!
		@brief Draw an input field for floats with an input pin and an
				output pin.
		@details Aligns the InputField text label and the Input Field body
				to the center of the node.
		@param _label The text label of the Input Field to display.
		@param _id The _id of the InputField for ImGui.
		@param _valueBuffer The float buffer to hold the value given by the user
				and displayed in the field.
		@param _inputFieldWidth The total width of the input field (text label
				and body).
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _inputPin The pin data used for the input pin to draw.
		@param _outputPin The pin data used for the output pin to draw.
		@param _pinColors The set of colors to cutomize both pins.
		@param _flags Flags to customize the behaviour of the input field.
				Note that not every ImGuiInputTextFlags have an effect on
				ImGui InputFloat widget.
		*/
		static bool NodeInputFloat_InOut(const char* _label, const std::size_t _id, float* _valueBuffer, 
			const float _inputFieldWidth, const float _startX, const float _drawLength,
			const NodePinData& _inputPin, const NodePinData& _outputPin, const ImVec4 _pinColors[],
			const ImGuiInputTextFlags _flags = ImGuiInputTextFlags_None);

		/*!
		@brief Draw an input field for text inputs. Contrary to other node
				inputs (e.g. NodeInputFloat_XXX) the assignment of the buffer
				value to the target value is handled in the function.
		@details Aligns the InputField text label and the Input Field body
				to the center of the node.
		@param _label The text label of the Input Field to display.
		@param _buffer The buffer char array where to store the input value.
		@param _bufferSize The byte size of the buffer char array.
		@param _inputFieldWidth The total width of the input field (text label
				and body).
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _flags Flags to customize the behaviour of the input field. Default
				to the flag @a _EnterReturnsTrue so users have to press enter for
				the buffer value to be assigned to the target.
		*/
		static bool NodeInputText(const char* _label, char* _target, char* _buffer,
			const std::size_t _bufferSize, 
			const float _inputFieldWidth, const float _startX, const float _drawLength,
			const ImGuiInputTextFlags _flags = ImGuiInputTextFlags_EnterReturnsTrue);

		/*!
		@brief Custom list box to display strings in nodes.
		@details This intends to reproduce ImGui::ListBox which is not
				 readily compatible with the node editor.
		@param _nslbData The data struct containing the items to draw.
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The width of the space where the list box is drawn.
				Used for alignment calculations.
		@param _widgetWidth The width of the whole list box (scroll bar
				included) in pixels.
		@param _itemViewHeight The number of items to display in the list box.
		@see It is using ::NodeScrollBar to control the scrolling of the items.
		*/
		static void NodeStringListBox(NodeListBoxStringData& _nslbData,
			const float _startX, const float _drawLength, const float _widgetWidth = 200.f,
			const unsigned short _itemViewHeight = 7);

		/*!
		@brief Draws a Text label with an input pin.
		@details Aligns the Text label to the left of the node (input side).
		@param _label The text label to display.
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _pin The pin data used for the input pin to draw.
		@param _pinColors The set of colors to cutomize the input pin.
		*/
		static void NodeText_In(const char* _label, const float _startX,
			const NodePinData& _pin, const ImVec4 _pinColors[]);

		/*!
		@brief Draws a Text Label with an input pin.
		@details Aligns the Text label to the left of the node (input side).
		@param _label The text label to display.
		@param _labelWidth The width of the text label to display.
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _pin The pin data used for the input pin to draw.
		@param _pinColors The set of colors to cutomize the input pin.
		*/
		static void NodeText_In(const char* _label, const float _labelWidth,
			const float _startX,
			const NodePinData& _pin, const ImVec4 _pinColors[]);

		/*!
		@brief Draws a Text label with an input pin and an output pin.
		@details Aligns the Text label to the center of the node.
		@param _label The text label to display.
		@param _labelWidth The width of the text label to display.
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _inputPin The pin data used for the input pin to draw.
		@param _outputPin The pin data used for the output pin to draw.
		@param _pinColors The set of colors to cutomize both pins.
		*/
		static void NodeText_InOut(const char* _label, const float _labelWidth,
			const float _startX, const float _drawLength,
			const NodePinData& _inputPin, const NodePinData& _outputPin,
			const ImVec4 _pinColors[]);

		/*!
		@brief Draws a Text Label with an output pin.
		@details Aligns the Text label to the right of the node (output side).
		@param _label The text label to display.
		@param _labelWidth The width of the text label to display.
		@param _startX The position from which the alignment calculations are
				done. Typically the left side of the node.
		@param _drawLength The distance where to draw the output pin relatively
				to @p _startX. Used for alignment calculations.
		@param _itemSpacingX The spacing to respect between the text and the
				output pin. Used for alignement calculations.
		@param _pin The pin data used for the output pin to draw.
		@param _pinColors The set of colors to cutomize the output pin.
		*/
		static void NodeText_Out(const char* _label, const float _labelWidth,
			const float _startX, const float _drawLength, const float itemSpacingX,
			const NodePinData& _pin, const ImVec4 _pinColors[]);
#pragma endregion

	};
}