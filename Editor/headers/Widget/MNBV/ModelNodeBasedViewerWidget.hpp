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
		/*!
		@brief The logic to decide what to draw when the user drops (from a drag
				& drop action) a payload in the area of this viewer.
		*/
		void HandleSimuDataRefDrop();

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