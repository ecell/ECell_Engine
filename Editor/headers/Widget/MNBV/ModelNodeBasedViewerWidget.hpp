#pragma once

/*!
@file
@brief The source for the windows where we can explore models and assets as a
		graph.
*/

#include "imgui_node_editor.h"

//forward declaration
namespace ECellEngine::Editor::Widget
{
	class ModelExplorerWidget;
}

#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"
#include "Utility/MNBV/NodeEditorDraw.hpp"

namespace ECellEngine::Editor::Widget::MNBV
{
	/*!
	@brief Displays the assets imported in a simulation space as a graph.
	@details Nodes may represent model assets, solvers, equations, species
			 and so on. Visibility of the nodes is partly controlled by the
			 ECellEngine::Editor::ModelHierarchyWidget of the parent window.
	@see It relies on a third party node editor:
		 https://github.com/thedmd/imgui-node-editor by thedmd
	@todo This window is relevant only in a Model Explorer. We must make sure 
		in the future that it cannot be docked anywhere else.
	*/
	class ModelNodeBasedViewerWidget : public Widget
	{
	private:

		enum MNBVState : unsigned char
		{
			MNBVState_None = 0,
			MNBVState_ImportAssetPopup = 1 << 0,
		};

		/*
		@brief 1-byte char to encode states of the widget.
		*/
		unsigned char utilityState = 0;

		/*!
		@brief A char array to hold the path to an asset to import.
		*/
		//char assetPathBuffer[256] = "C:/Users/EliottJacopin/Documents/Source/External/Papers/p53/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod2.xml";
		char assetPathBuffer[256] = "C:/Users/EliottJacopin/Documents/Source/External/Papers/Novak-et-al_FissionYeast_CellCycle/BIOMD0000000111_L3V2.xml";

		/*!
		@brief A char array to hold the name of the asset node to be drawn in the
				editor and that matches the imported asset.
		*/
		//char assetNameBuffer[64] = "p53_L3V2_mod2.xml";
		char assetNameBuffer[64] = "FissionYeast_CellCycle_L3V2.xml";

		/*!
		@brief The vector that contains the arguments for the ECellEngine::IO::AddModuleCommand
		*/
		std::vector<std::string> addModuleCommandArray = { "addModule", "simulationID", "path", "name"};

		/*!
		@brief A buffer for the mouse position since we use it a few times.
		*/
		ImVec2 mousePos = ImVec2(0, 0);

		/*!
		@brief The logic to decide what to draw when the user drops (from a drag
				& drop action) a payload in the area of this viewer.
		*/
		void HandleSimuDataRefDrop();

		/*!
		@brief Draws the popup window used to inform the path of the asset to
				import in the current simulation space.
		*/
		void DrawImportAssetPopup();

		void DrawNodes();

		void DrawNodesPopupMenu();

	public:

		/*!
		@brief The NodeEditorContext this model viewer is using.
		@remark This is set on construction and unlikely to be changed.
		*/
		ax::NodeEditor::EditorContext* editorCtxt;

		/*!
		@brief Buffer pointer to the current context.
		@remark This will be set at every ::Draw() call to the current context.
		@see ECellEngine::Editor::Utility::MNBV::GetCurrentMNBVContext()
		*/
		ModelNodeBasedViewerContext* currentMNBVContext = nullptr;

		ModelNodeBasedViewerWidget(Editor* _editor) :
			Widget(_editor), editorCtxt(ax::NodeEditor::CreateEditor())
		{
			
		}

		void Awake() override;

		void Draw() override;
	};
}