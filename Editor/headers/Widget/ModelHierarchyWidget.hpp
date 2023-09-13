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
			//HierarchyContext_RenamingDone = 1 << 1,
			//HierarchyContext_RenamedNodeWasOpen = 1 << 2,
			HierarchyContext_RightClick = 1 << 3,
			//HierarchyContext_RightClickOnBackground = 1 << 4,
			//HierarchyContext_RightClickOnSimulationNode = 1 << 5,
			//HierarchyContext_RightClickOnMNBVCtxtNode = 1 << 6,

			//to continue as needed

		};

		enum ContextItem
		{
			ContextItem_None = 0,
			ContextItem_Background = 1 << 0, //right click on background
			ContextItem_Simulation = 1 << 1, //right click on a simulation node
			ContextItem_DataState = 1 << 2, //right click on the data state node of a simulation node
			ContextItem_DataStateLeaf = 1 << 3, //right click on a data state leaf node
			ContextItem_DataStateLeafs = 1 << 4, //right click on the node containing the leafs of data state elements
			ContextItem_MNBVCtxt = 1 << 5, //right click on a MNBVContext node
			ContextItem_MNBVCtxts = 1 << 6, //right click on the node containing the MNBVContext nodes
			ContextItem_MNBVCtxtNodeLeaf = 1 << 7, //right click on a leaf node in a MNBVContext node
			ContextItem_MNBVCtxtNodeLeafs = 1 << 8, //right click on the node containing the leafs of MNBVContext nodes
		};

		/*!
		@brief A function object to access the name of an element encapsulated
				in a shared pointer in a list.
		@details The element MUST implement a <em>char* GetName()</em> method.
		*/
		struct NameAccessorBySPtr
		{
			template<typename T>
			char* operator()(std::shared_ptr<T> _ptr) const noexcept
			{
				return _ptr->GetName();
			}
		};

		/*!
		@brief A function object to access the name of an element in a list
				by reference.
		@details The element MUST implement a <em>char* GetName()</em> method.
		*/
		struct NameAccessorByRef
		{
			template<typename T>
			char* operator()(T& _ref) const noexcept
			{
				return _ref.GetName();
			}
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
		unsigned short contextNodeIdx = 0;

		unsigned char simuCount = 0;

		unsigned int mnbvCtxtCount = 0;

		ContextItem contextItemType = ContextItem_None;

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
			ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
		
		/*!
		@brief Draws the context menu upon right clicking on the hierarchy.
		*/
		void DrawContextMenu();

		/*!
		@brief Draws the whole tree making the hierarchy
		*/
		void DrawHierarchy();

		/*!
		@brief Uses the content of a list to draw tree nodes behaving as leafs.
		@details The elements in the list MUST implement a <em>char* GetName()</em>
				 method
		@param _leafsListName The name of the list to display. This will be used as
								the name of the tree node containing every leaf node.
		@param _mnbvCtxt The MNBV context to draw the hierarchy of.
		@param _leafs The list of elements to display.
		@paramt LeafType The type of the elements in the list.
		@paramt NameAccessorType The type of the function object to access the name
								 of the elements in the list.
		@see NameAccessorBySPtr and NameAccessorByRef
		*/
		template<typename LeafType, typename NameAccessorType>
		void DrawHierarchyLeafsList(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::vector<LeafType>& _leafs, NameAccessorType& _nameAccessor);
		
		/*!
		@brief Uses the content of an unordered_map to draw tree nodes behaving as leafs.
		@details The elements in the unordered_map MUST implement a <em>char* GetName()</em>
				 method
		@param _leafsListName The name of the list to display. This will be used as
								the name of the tree node containing every leaf node.
		@param _mnbvCtxt The MNBV context to draw the hierarchy of.
		@param _leafs The unordered_map of elements to display.
		@paramt LeafKeyType The type of the keys of the unordered_map.
		@paramt LeafType The type of the elements in the unordered_map.
		@paramt NameAccessorType The type of the function object to access the name
								 of the elements in the unordered_map.
		@see NameAccessorBySPtr and NameAccessorByRef
		*/
		template<typename LeafKeyType, typename LeafType, typename NameAccessorType>
		void DrawHierarchyLeafsUMap(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::unordered_map<LeafKeyType, LeafType>& _leafs, NameAccessorType& _nameAccessor);

		/*!
		@brief Draws the subparts of the hierarchy corresponding to the MNBV
				context/
		@param _mnbvCtxt The MNBV context to draw the hierarchy of.
		*/
		void DrawMNBVCtxtHierarchy(MNBV::ModelNodeBasedViewerContext& _mnbvCtxt);
		
		/*!
		@brief Draws the subparts of the hierarchy corresponding to the
				simulation.
		@param _simulation The simulation to draw the hierarchy of.
		*/
		void DrawSimulationHierarchy(ECellEngine::Core::Simulation* _simulation);
		
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
	};

}