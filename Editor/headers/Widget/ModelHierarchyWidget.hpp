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
		enum HierarchyContext : unsigned char
		{
			HierarchyContext_None = 0,
			HierarchyContext_RenamingInProgress = 1 << 0,
			HierarchyContext_RightClick = 1 << 1,
			HierarchyContext_DnD = 1 << 2,

			//to continue as needed

		};

		/*!
		@brief The binary flags to indicate the type of the item currently being
				acted upon.
		*/
		enum HierarchyLevel : unsigned int
		{
			HierarchyLevel_None = 0,
			HierarchyLevel_Background = 1 << 0, //background
			HierarchyLevel_Simulation = 1 << 1, //a simulation node
			HierarchyLevel_DataState = 1 << 2, //the data state node of a simulation node
			HierarchyLevel_Leaf = 1 << 3, //a leaf node
			HierarchyLevel_Leafs = 1 << 4, //the node containing the leafs
			HierarchyLevel_MNBVCtxt = 1 << 5, //a MNBVContext node
			HierarchyLevel_MNBVCtxts = 1 << 6, //the node containing the MNBVContext nodes

			HierarchyLevel_Equations = 1 << 7, //equation nodes & data (it's either since we also have HierarchyLevel_DataState & HierarchyLevel_MNBVCtxt to know which once it really is)
			HierarchyLevel_Parameters = 1 << 8, //parameter nodes & data
			HierarchyLevel_Reactions = 1 << 9, //reaction nodes & data
			HierarchyLevel_Species = 1 << 10, //species nodes & data

			HierarchyLevel_ArithmeticNodes = 1 << 11, //arithmetic nodes
			HierarchyLevel_AssetNodes = 1 << 12, //asset nodes
			HierarchyLevel_LinePlotNodes = 1 << 13, //line plot nodes
			HierarchyLevel_LogicNodes = 1 << 14, //logic nodes
			HierarchyLevel_ModifyDataStateValueEventNodes = 1 << 15, //modify data state value events
			HierarchyLevel_SolverNodes = 1 << 16, //solver nodes
			HierarchyLevel_TimeNodes = 1 << 17, //time nodes
			HierarchyLevel_TriggerNodes = 1 << 18, //trigger nodes
			HierarchyLevel_ValueNodes = 1 << 19, //value nodes


		};

		/*!
		@brief A function object to access the name of an element encapsulated
				in a shared pointer.
		@details The element MUST implement a <em>char* GetName()</em> method.
		*/
		struct NameGetterBySPtr
		{
			template<typename T>
			char* operator()(std::shared_ptr<T> _ptr) const noexcept
			{
				return _ptr->GetName();
			}
		};

		/*!
		@brief A function object to set the name of an element encapsulated in a
				shared pointer.
		@details The element MUST implement a <em>void SetName(const char*)</em>
				 method.
		*/
		struct NameSetterBySPtr
		{
			template<typename T>
			void operator()(std::shared_ptr<T> _ptr, const char* _newName) noexcept
			{
				_ptr->SetName(_newName);
			}
		};

		/*!
		@brief A function object to access the name of an element by iterator (or raw pointer).
		@details The element MUST implement a <em>char* GetName()</em> method.
		*/
		struct NameGetterByIt
		{
			template<typename T>
			char* operator()(T _it) const noexcept
			{
				return _it->GetName();
			}
		};

		/*!
		@brief A function object to set the name of an element by iterator (or raw pointer).
		@details The element MUST implement a <em>void SetName(const char*)</em>
				 method.
		*/
		struct NameSetterByIt
		{
			template<typename T>
			void operator()(T _it, const char* _newName) noexcept
			{
				_it->SetName(_newName);
			}
		};


	private:

		/*!
		@brief A tracker to the index of the node currently being drawn.
		@details Effectively, it follows the number of nodes. Therefore, it is also
				 the index of the node in the hierarchy tree (depth first).
		*/
		unsigned short globalNodeCount = 0;

		/*!
		@brief A buffer of the index of the node currently being acted upon.
		@details This index is the position of the item within every items draw
				in this hierarchy. In effect, when the user interacts with a node
				in the hierarchy, it is set equal to ::globalNodeCount.
		*/
		unsigned short globalNodeIdx = 0;

		/*!
		@brief A tracker to the index of the simulation currently being drawn.
		@details It follows the index of the simulation in ::simuManager.simulations.
		*/
		unsigned char simuCount = 0;

		/*!
		@brief A buffer of the index of the simulation currently being acted upon or
				for which a child node is being acted upon.
		@details When the user interacts with a simulation node in the hierarchy, or
				 one of its children nodes,it is set equal to ::simuCount.
		*/
		unsigned char simuIdx = 0;

		/*!
		@brief A tracker to the index of the MNBV context currently being drawn.
		@details It follows the index of the MNBV context in ::mnbvCtxts.
		*/
		unsigned short mnbvCtxtCount = 0;

		/*!
		@brief A buffer of the index of the MNBV context currently being acted upon or
				for which a child node is being acted upon.
		@details When the user interacts with a MNBV context node in the hierarchy, or
				 one of its children nodes,it is set equal to ::mnbvCtxtCount.
		*/
		unsigned short mnbvCtxtIdx = 0;

		/*!
		@brief A buffer for the type of the item currently being acted upon.
		@details This takes the value of ::hierarchyLevelAccumulator when the user
				 interacts with a node in the hierarchy.
		*/
		unsigned int hierarchyLevel = HierarchyLevel_None;

		/*!
		@brief A buffer for the tree level of the item currently being drawn.
		@details Flags of the ::HirarchyLevel enum are set and cleared whenever
				 entering or exiting a node in the hierarchy. This way, we know
				 the path to the item currently being drawn.
		*/
		unsigned int hierarchyLevelAccumulator = HierarchyLevel_None;

		/*!
		@brief A convenienve pointer to store access to the underlying data
				encapsulated in the tree node of the hierarchy currently being
				interacted with.
		*/
		const void* ctxtNodePayload = nullptr;

		/*!
		@brief The buffer to store the new name of the item being renamed.
		*/
		char renamingBuffer[64] = { 0 };

		/*!
		@brief The convenience byte to represent the state of the interaction
				between the user and the hierarchy with the flags defined in
				::HierarchyContext.
		*/
		unsigned char hierarchyCtxt = HierarchyContext_None;

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
		@brief A utility function to get the name of the MNBV node type accessible
				in the hierarchy.
		@details This uses a subset of the enum ::HierarchyLevel.
		@returns Standardized names. The possible values are:
					- "Equation"
					- "Parameter"
					- "Reaction"
					- "Species"
					- "Arithmetic"
					- "Asset"
					- "LinePlot"
					- "Logic"
					- "ModifyDataStateValueEvent"
					- "Solver"
					- "Time"
					- "Trigger"
					- "Value"
		*/
		char* GetNodeTypeName() noexcept;

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
		@param _leafs The list of elements to display.
		@paramt LeafType The type of the elements in the list.
		@paramt NameGetterType The type of the function object to access the name
								 of the elements in the list.
		@paramt NameSetterType The type of the function object to set the name
								 of the elements in the list.
		@see NameGetterBySPtr and NameGetterByRef, NameSetterBySPtr and NameSetterByRef
		*/
		template<typename LeafType, typename NameGetterType, typename NameSetterType>
		void DrawHierarchyLeafsList(const char* _leafsListName, std::vector<LeafType>& _leafs,
			NameGetterType& _nameGetter, NameSetterType& _nameSetter);
		
		/*!
		@brief Uses the content of an unordered_map to draw tree nodes behaving as leafs.
		@details The elements in the unordered_map MUST implement a <em>char* GetName()</em>
				 method
		@param _leafsListName The name of the list to display. This will be used as
								the name of the tree node containing every leaf node.
		@param _leafs The unordered_map of elements to display.
		@paramt LeafKeyType The type of the keys of the unordered_map.
		@paramt LeafType The type of the elements in the unordered_map.
		@paramt NameGetterType The type of the function object to access the name
								 of the elements in the unordered_map.
		@paramt NameSetterType The type of the function object to set the name
								 of the elements in the unordered_map.
		@see ::NameGetterBySPtr, ::NameGetterByRef, ::NameSetterBySPtr and ::NameSetterByRef
		*/
		template<typename LeafKeyType, typename LeafType, typename NameGetterType, typename NameSetterType>
		void DrawHierarchyLeafsUMap(const char* _leafsListName, const std::unordered_map<LeafKeyType, LeafType>& _leafs,
			NameGetterType& _nameGetter, NameSetterType& _nameSetter);

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
		@brief Handles the logic of the drag and drop of a leaf node in the
				hierarchy.
		*/
		void HandleDnD();

		/*!
		@brief A convenience function to assign the values of ::globalNodeIdx,
					::simuIdx and ::mnbvCtxtIdx to the value of ::globalNodeCount,
					::simuCount and ::mnbvCtxtCount respectively.
		*/
		void TrackContextItem() noexcept;

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