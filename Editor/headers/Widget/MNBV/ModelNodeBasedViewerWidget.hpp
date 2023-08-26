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
		@brief The index of the ModelNodeBasedViewerContext in
				ModelExplorerWidget::mnbvCtxts that this model viewer is using.
		*/
		std::size_t mnbvCtxIdx = 0;

		/*!
		@brief The logic to decide what to draw when the user drops (from a drag
				& drop action) a payload in the area of this viewer.
		*/
		void HandleSimuDataRefDrop();

	public:

		/*!
		@brief The reference to the parent window.
		*/
		ModelExplorerWidget* rootExplorer;

		/*!
		@brief The index of the NodeEditorContext this model viewer is using.
		*/
		ax::NodeEditor::EditorContext* editorCtxt;

		ModelNodeBasedViewerWidget(Editor& _editor, ModelExplorerWidget* _rootExplorer) :
			Widget(_editor), rootExplorer{ _rootExplorer }, editorCtxt(ax::NodeEditor::CreateEditor())
		{
			//Updates global style values that we want to be applied to everything in our use case.
			//We don't use the Push/Pop API on purpose because we will not change those values in the
			//future.
			ax::NodeEditor::SetCurrentEditor(editorCtxt);

			ax::NodeEditor::Style& axStyle = ax::NodeEditor::GetStyle();
			Style::NodeEditorStyle& neStyle = Style::EditorStyle::GetMNBVStyle();
			axStyle.NodeRounding = neStyle.nodeRounding;
			axStyle.NodeBorderWidth = neStyle.nodeBorderWidth;
			axStyle.HoveredNodeBorderWidth = neStyle.hoveredNodeBorderWidth;
			axStyle.SelectedNodeBorderWidth = neStyle.selectedNodeBorderWidth;
			axStyle.PinBorderWidth = neStyle.pinBorderWidth;
			axStyle.PinRounding = neStyle.pinRounding;

			ax::NodeEditor::SetCurrentEditor(nullptr);
		}

		/*!
		@remarks We are using ::rootExplorer which is forward declared so the
				 definition of this destructor is in the .cpp.
		*/
		~ModelNodeBasedViewerWidget()
		{
			ax::NodeEditor::DestroyEditor(editorCtxt);
		}

		void Awake() override;

		void Draw() override;

		
	};
}