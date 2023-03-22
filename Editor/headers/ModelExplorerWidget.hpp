#pragma once

#include <string>
#include <vector>

#include "imgui_node_editor.h"

#include "Logger.hpp"
#include "NodeEditorUtility.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	/*!
	@brief The window that contains the GUI to explore the assembled model of a
			simulation.
	@details This window displays the model as a network of connected nodes.
			 Nodes may represent model assets, solvers and the data state.
	@see It relies on a third party node editor: https://github.com/thedmd/imgui-node-editor by thedmd
	*/
	class ModelExplorerWidget : public Widget
	{
	private:

		/*!
		@brief 1-byte char to encode states of the widget
		@details Bit 0 encodes the open/close status of the popup to import an asset
				 from file.

				 Bit 1 encodes the open/close status of the popup to add a solver.
		*/
		unsigned char utilityState = 0;

		/*!
		@brief A char array to hold the path to an asset to import. 
		*/
		char assetPathBuffer[256] = "C:/Users/eliot/Documents/Source/External/Papers/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod2.xml";
		
		/*!
		@brief A char array to hold the name of the asset node to be drawn in the
				editor and that matches the imported asset.
		*/
		char assetNameBuffer[256] = "p53_L3V2_mod2.xml";

		/*!
		  @brief Flags for the model explorer window.
		  @details Enables the MenuBar.
		*/
		const ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_MenuBar;

		/*!
		@brief Flags for the custom popup windows spawned from this widget.
		*/
		const ImGuiWindowFlags popupWindowFlags =
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;

		/*!
		@brief The vector that contains the arguments for the 
				ECellEngine::IO::AddModuleCommand
		*/
		std::vector<std::string> addModuleCommandArray = {"addModule", "0", "path"};

		/*!
		@brief The vector that contains the arguments for the 
				ECellEngine::IO::AddSolverCommand
		*/
		std::vector<std::string> addSolverCommandArray = {"addSolver", "0", "solverName"};

		/*!
		@brief The node editor context.
		*/
		ax::NodeEditor::EditorContext * nodeEditorCtxt = nullptr;
		std::size_t uniqueIdx;

		/*!
		@brief The list of asset nodes.
		@details It contains the information used to draw the nodes corresponding
				 to each asset imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::AssetNodeInfo> assetNodes;

		std::vector<ECellEngine::Editor::Utility::LinkInfo> links;// List of live links. It is dynamic unless you want to create read-only view over nodes.
		
		/*!
		@brief Adds an ECellEngine::Editor::Utility::AssetNodeInfo in ::assetNodes.
		*/
		void AddAssetNode(const char* _name);

		/*!
		@brief Draws the popup window used to select which solver to add in the
				current simulation space.
		*/
		void DrawAddSolverPopup();

		/*!
		@brief Draws the popup window used to inform the path of the asset to
				import in the current simulation space.
		*/
		void DrawImportAssetPopup();

		/*!
		@brief Draws the node display space to explore the simulation.
		*/
		void DrawModelExplorer();

		/*!
		@brief Draw every options of the Model Explorer's menu bar.
		*/
		void DrawMenuBar();

		/*!
		@brief Removes as asset node from ::assetNodes
		@param _idx The idex of the node to erase from ::assetNodes.
		*/
		inline void RemoveAssetNode(const std::size_t _idx)
		{
			assetNodes.erase(assetNodes.begin() + _idx);
		}

	public:
		ModelExplorerWidget(Editor& _editor) :
			Widget(_editor)
		{
			uniqueIdx = 0;
			ax::NodeEditor::Config nodeConfig;
			nodeEditorCtxt = ax::NodeEditor::CreateEditor(&nodeConfig);
		}

		~ModelExplorerWidget()
		{
			ax::NodeEditor::DestroyEditor(nodeEditorCtxt);
		}

		inline void Awake() override {};

		void Draw() override;
	};
}