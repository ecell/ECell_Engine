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

#include "NodeEditorDraw.hpp"
#include "Widget.hpp"

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
		@brief The index of the NodeEditorContext in ModelExplorerWidget::nodeEditorCtxts
				that this model viewer is using.
		*/
		std::size_t neCtxtIdx = 0;

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

		ModelNodeBasedViewerWidget(Editor& _editor, ModelExplorerWidget* _rootExplorer) :
			Widget(_editor), rootExplorer{ _rootExplorer }
		{
			
		}

		/*!
		@remarks We are using ::rootExplorer which is forward declared so the
				 definition of this destructor is in the .cpp.
		*/
		~ModelNodeBasedViewerWidget();

		void Awake() override;

		void Draw() override;

		
	};
}