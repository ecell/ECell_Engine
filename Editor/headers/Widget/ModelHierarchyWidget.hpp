#pragma once
 /*!
 @file
 @brief Source of the widget to display the tree of assets contained in a
		simulation.
 */

//forward declaration
namespace ECellEngine::Editor
{
	class ModelExplorerWidget;
}

#include "Core/Simulation.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor::Widget
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
		@brief A boolean to detect whether an item in the hirearchy is currently
				being renamed.
		*/
		bool renamingInProgress = false;

		/*!
		@brief The index of the item currently being renamed.
		@details This index is the position of the item within every items draw
				in this hierarchy.
		*/
		short renamingIdx = 0;

		/*!
		@brief The pointer to the simulation for which we want to display the assets
				(SBML Module and Solvers).
		*/
		ECellEngine::Core::Simulation* simulation = nullptr;

		/*!
		@brief The flags to transform normal tree nodes into leafs in the
				hierarchy.
		*/
		const ImGuiTreeNodeFlags leafNodeFlags =
			ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	public:
		ModelHierarchyWidget(Editor* _editor) :
			Widget(_editor)
		{

		}

		/*!
		@brief Sets the value for ::simulation.
		@details Gets the simulation from the simulation manager of the engine.
		@param _simulationIndex The index of the target simulation in the list 
				of all simulations in ECellEngine::Core::SimulationsManager::simulations.
		*/
		void SetSimulation(std::size_t _simulationIndex);

		void Awake() override;

		void Draw() override;
	};

}