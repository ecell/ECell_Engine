#pragma once

#include "imgui_internal.h"
#include "NodeEditorData.hpp"

#include "Logger.hpp"

namespace ECellEngine::Editor::Utility
{
	/*!
	@brief Struct with public static logic to help building the NodeEditor.
	@remarks Contains code from: https://github.com/thedmd/imgui-node-editor/blob/master/examples/basic-interaction-example/basic-interaction-example.cpp
	*/
	struct NodeEditorDraw
	{
#pragma region Nodes
		/*!
		@brief Draws a node to display and access the content of an
				ECellEngine::Data::Module.
		@param _name The name of the node. It will appear in the header of the
				node in the editor.
		@param _assetNodeInfo The struct with information about what to draw..
		*/
		static void AssetNode(const char* _name, AssetNodeData& _assetNodeInfo);

		/*!
		@brief Draws a node to display and access the content of a
				ECellEngine::Solvers::Solver.
		@param _name The name of the node. It will appear in the header of the
				node in the editor.
		@param _solverNodeInfo The struct with information about what to draw.
		*/
		static void SolverNode(const char* _name, const SolverNodeData& _solverNodeInfo);
#pragma endregion

#pragma region Node Pins
		/*!
		@brief Draw a basic input pin symbolized by an arrow. 
		@param _pinData The struct containing the information about the
				pin to be drawn as an input pin.
		*/
		static void InputPin(const NodePinData& _pinData);

		/*!
		@brief Draw a basic output pin symbolized by an arrow.
		@param _pinData The struct containing the information about the
				pin to be drawn as an output pin.
		*/
		static void OutputPin(const NodePinData& _pinData);

		/*!
		@brief The basic logic to create links between input and output
				pins of nodes.
		@param _id The current value of the ID counter for links.
		@param _links The current list of links.
		*/
		static void LinkCreation(int& _id, std::vector<ECellEngine::Editor::Utility::LinkData>& _links);

		/*!
		@brief The basic logic to destroy links between nodes.
		@param _links The current list of links.
		*/
		static void LinkDestruction(std::vector<ECellEngine::Editor::Utility::LinkData>& _links);
#pragma endregion

#pragma region Custom Node Widget
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
		@brief Custom collapsing header for nodes.
		@details This intends to reproduce ImGui::CollapsingHeader but without
				 relying on ImGui's tree nodes API. Because it is not possible
				 (as of ImGui 1.89) to control the width of a tree node which
				 creates a visual artifact: the node extends infinitely because
				 in the Node Editor virtual area has not limits.
		@param _label The text to display in the collapsing header.
		@param _utilityState The reference to the character containing the encoding
				of the the open/close state of this header.
		@param _stateBitPos The position of the bit in @p _utilityState that
				encodes the open/close state of this header
		@param _size The size of the button representing the collapsing header.
		*/
		inline static bool NodeCollapsingHeader(const char* _label,
			unsigned char& _utilityState, const short _stateBitPos, const ImVec2& _size = ImVec2(0, 0))
		{
			if (ImGui::Button(_label, _size))
			{
				_utilityState ^= 1 << _stateBitPos;
			}
			return _utilityState >> _stateBitPos & 1;
		}


		/*!
		@brief Logic to draw headers of nodes.
		*/
		static void NodeHeader(const char* _type, const char* _name, const float _width = 200.f, const short _height = 1);

		/*!
		@brief Draws a horizontal line of a set width and thickness.
		@details Intends to replace ImGui::Separator() which spans infinitely in width within
				 the node editor.
		*/
		static void NodeHorizontalSeparator(const float _width, const float _thickness = 1.f);

		/*!
		@brief Custom list box to display strings in nodes.
		@details This intends to reproduce ImGui::ListBox which is not
				 readily compatible with the node editor.
		@param _id A char-based label to uniquelly identify the scroll bar
				of the list box. This is relevant in the case that multiple
				scroll bars are drawn in the same node.
		@param _lbsData
		@param _widgetWidth The width of the whole list box (scroll bar
				included) in pixels.
		@param _itemViewHeight The number of items to display in the list box.
		@see It is using ::NodeScrollBar to control the scrolling of the items.
		*/
		static void NodeStringListBox(const char* _id, NodeListBoxStringData& _lbsData,
			const float _widgetWidth = 200.f, const short _itemViewHeight = 7);


		/*!
		@brief Pops the number of styles pushed with ::PushScrollBarStyle(ImGuiStyle& _style)
		*/
		static void PopScrollBarStyle();

		/*!
		@brief Pushes a bunch of styles to match ImGui's default Scroll Bar style.
				Can be used with sliders in nodes.
		@details Don't forget to call ::PopScrollBarStyle() after you've drawn the
				 slider.
		@see ::PopScrollBarStyle()
		*/
		static void PushScrollBarStyle(ImGuiStyle& _style);

#pragma endregion

	};
}