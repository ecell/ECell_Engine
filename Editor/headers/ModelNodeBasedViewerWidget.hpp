#pragma once

#include "imgui_node_editor.h"

//forward declaration
namespace ECellEngine::Editor
{
	class ModelExplorerWidget;
}

#include "NodeEditorDraw.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
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

		std::size_t uniqueIdx = 0;

		/*!
		@brief The list of asset nodes.
		@details It contains the information used to draw the nodes corresponding
				 to each asset imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::AssetNodeData> assetNodes;

		std::vector<ECellEngine::Editor::Utility::SpeciesNodeData> speciesNodes;

		/*!
		@brief The list of solver nodes.
		@details It contains the information used to draw the nodes corresponding
				 to each solvers imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::SolverNodeData> solverNodes;

		std::vector<ECellEngine::Editor::Utility::LinkData> links;// List of live links. It is dynamic unless you want to create read-only view over nodes.

		ModelNodeBasedViewerWidget(Editor& _editor, ModelExplorerWidget* _rootExplorer) :
			Widget(_editor), rootExplorer{ _rootExplorer }
		{
			
		}

		/*!
		@remarks We are using ::rootExplorer which is forward declared so the
				 definition of this destructor is in the .cpp.
		*/
		~ModelNodeBasedViewerWidget();

		/*!
		@brief Adds an ECellEngine::Editor::Utility::AssetNodeInfo in ::assetNodes.
		@details It has for consequence to draw an asset node in the editor.
		*/
		void AddAssetNode(const std::size_t _dataIdx);

		/*!
		@brief Adds an ECellEngine::Editor::Utility::SolverNodeInfo in ::solverNodes.
		@details It has for consequence to draw a solver node in the editor.
		*/
		void AddSolverNode(const std::size_t _dataIdx);

		void Awake() override;

		void Draw() override;

		/*!
		@brief Removes as asset node from ::assetNodes
		@param _idx The index of the node to erase from ::assetNodes.
		*/
		inline void RemoveAssetNode(const std::size_t _idx)
		{
			assetNodes.erase(assetNodes.begin() + _idx);
		}
	};
}