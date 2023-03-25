#pragma once
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
		@brief Draws a node to display and access the content of an
				ECellEngine::Data::Module.
		@param _name The name of the node. It will appear in the header of the
				node in the editor.
		@param _assetNodeInfo The struct with information about what to draw..
		*/
		static void AssetNode(const char* _name, const AssetNodeData& _assetNodeInfo);

		/*!
		@brief Draws a node to display and access the content of a
				ECellEngine::Solvers::Solver.
		@param _name The name of the node. It will appear in the header of the
				node in the editor.
		@param _solverNodeInfo The struct with information about what to draw.
		*/
		static void SolverNode(const char* _name, const SolverNodeData& _solverNodeInfo);

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