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

namespace ECellEngine::Editor::Widget::MNBV
{
	class ModelNodeBasedViewerContext;
}

#include "Core/SimulationsManager.hpp"
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
		enum HierarchyContext
		{
			HierarchyContext_None = 0,
			HierarchyContext_RenamingInProgress = 1 << 0,
			HierarchyContext_RenamingDone = 1 << 1,
			HierarchyContext_RenamedNodeWasOpen = 1 << 2,
			HierarchyContext_RightClick = 1 << 3,
			HierarchyContext_RightClickOnBackground = 1 << 4,
			HierarchyContext_RightClickOnSimulationNode = 1 << 5,
			HierarchyContext_RightClickOnMNBVCtxtNode = 1 << 6,

			//to continue as needed

		};

	private:

		/*!
		@brief A buffer to ID the node currently being drawn.
		@details Effectively, it follows the number of nodes. Therefore, it is also
				 the index of the node in the hierarchy tree (depth first).
		*/
		short nodeID = 0;

		/*!
		@brief The index of the item currently being acted upon.
		@details This index is the position of the item within every items draw
				in this hierarchy.
		*/
		short contextNodeIdx = 0;

		/*!
		@brief The buffer to store the new name of the item being renamed.
		*/
		char renamingBuffer[64] = { 0 };

		/*!
		@brief The binary flags to store the context of the hierarchy.
		*/
		unsigned char hierarchyCtxt = HierarchyContext_None;

		/*!
		@brief The open/close state of the nodes in the hierarchy (except the
				leafs, of course).
		*/
		std::vector<bool> openedNodes;

		/*!
		@brief The pointer to the simulationManager to display all simulations.
		@details Simulations manager is a singleton but this widget will use a 
				 pointer to it to avoid calling the singleton every frame.
		*/
		ECellEngine::Core::SimulationsManager& simuManager;

		/*!
		@brief The pointer to the list of contexts loaded in the model explorer
				containing this hierarchy.
		@details This pointer is kept valid by the model explorer widget.
		@see ECellEngine::Editor::ModelExplorerWidget::mnbvCtxts
		*/
		std::vector<MNBV::ModelNodeBasedViewerContext>* mnbvCtxts = nullptr;

		/*!
		@brief The flags to transform normal tree nodes into leafs in the
				hierarchy.
		*/
		const ImGuiTreeNodeFlags leafNodeFlags =
			ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	public:
		ModelHierarchyWidget(Editor* _editor) :
			Widget(_editor), simuManager(ECellEngine::Core::SimulationsManager::GetSingleton())
		{

		}

		/*!
		@brief Sets the pointer to the list of contexts loaded in the model
				explorer containing this hierarchy.
		@see ::mnbvCtxts
		*/
		inline void SetMNBVCtxts(std::vector<MNBV::ModelNodeBasedViewerContext>* _mnbvCtxts) noexcept
		{
			mnbvCtxts = _mnbvCtxts;
		}

		void Awake() override;

		void Draw() override;

		/*!
		@brief Draws the context menu upon right clicking on the hierarchy.
		*/
		void DrawContextMenu();

		/*!
		@brief Draws the whole tree making the hierarchy
		*/
		void DrawHierarchy();

		/*!
		@brief Draws the subparts of the hierarchy corresponding to the MNBV
				context/
		@param _mnbvCtxt The MNBV context to draw the hierarchy of.
		*/
		void DrawMNBVCtxtHierarchy(MNBV::ModelNodeBasedViewerContext& _mnbvCtxt);
		
		/*!
		@brief Draws the subparts of the hierarchy corresponding to the
				simulation.
		@param _out_mnbvCtxtStartIdx The index of the current MNBV context in the
									 list of MNBV contexts of ::simuManager. This
									 used to count the number of context that have
									 been drawn in between simulation nodes.
		@param _simulation The simulation to draw the hierarchy of.
		*/
		void DrawSimulationHierarchy(int& _out_mnbvCtxtStartIdx, ECellEngine::Core::Simulation* _simulation);
		
		/*!
		@brief Logic to handle the renaming of a node in the hierarchy.
		@details This is called when the user double clicks on a node in the
				 hierarchy. It will display a text field to enter the new name
				 of the node. The user can then press enter to validate the
				 change or click ouside to cancel it.
		@param _nameBuffer The buffer to store the new name of the node.
		@return @a True if the user has validated the change by pressing enter;
				@a false otherwise.
		*/
		bool TreeNodeRename(char* _nameBuffer);
	};

}