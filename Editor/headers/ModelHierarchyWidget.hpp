#pragma once

//forward declaration
namespace ECellEngine::Editor
{
	class ModelExplorerWidget;
}

#include "Widget.hpp"

namespace ECellEngine::Editor
{
	/*!
	@brief A widget to display the tree of data added in a simulation space.
	@details Contains links to all assets and solvers added present in the
			 simulation. From there, we can show/hide the corresponding
			 nodes in the ECellEngine::Editor::ModelNodeBasedViewerWidget.
	@todo This window is relevant only in a Model Explorer. We must make sure
		in the future that it cannot be docked anywhere else.
	*/
	class ModelHierarchyWidget : public Widget
	{
	private:
		/*!
		@brief The reference to the parent window.
		*/
		ModelExplorerWidget* rootExplorer;

	public:
		ModelHierarchyWidget(Editor& _editor, ModelExplorerWidget* _rootExplorer) :
			Widget(_editor), rootExplorer{ _rootExplorer }
		{

		}

		inline void Awake() override {};

		void Draw() override;
	};

}