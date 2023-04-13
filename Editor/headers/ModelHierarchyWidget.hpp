#pragma once

//forward declaration
namespace ECellEngine::Editor
{
	class ModelExplorerWidget;
}

#include "Simulation.hpp"
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

		ECellEngine::Core::Simulation* simulation = nullptr;

		/*!
		@brief The flags to transform normal tree nodes into leafs in the
				hierarchy.
		*/
		const ImGuiTreeNodeFlags leafNodeFlags =
			ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	public:
		ModelHierarchyWidget(Editor& _editor) :
			Widget(_editor)
		{

		}

		void SetSimulation(std::size_t _simulationIndex);

		void Awake() override;

		void Draw() override;
	};

}