#pragma once

#include "imgui_node_editor.h"

//forward declaration
namespace ECellEngine::Editor
{
	class ModelExplorerWidget;
}

#include "NodeEditorUtility.hpp"
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
	public:

		/*!
		@brief The reference to the parent window.
		*/
		ModelExplorerWidget* rootExplorer;

		/*!
		@brief The node editor context.
		*/
		ax::NodeEditor::EditorContext* nodeEditorCtxt = nullptr;
		std::size_t uniqueIdx = 0;

		/*!
		@brief The list of asset nodes.
		@details It contains the information used to draw the nodes corresponding
				 to each asset imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::AssetNodeInfo> assetNodes;

		std::vector<ECellEngine::Editor::Utility::LinkInfo> links;// List of live links. It is dynamic unless you want to create read-only view over nodes.

		ModelNodeBasedViewerWidget(Editor& _editor, ModelExplorerWidget* _rootExplorer) :
			Widget(_editor), rootExplorer{ _rootExplorer }
		{
			
		}

		~ModelNodeBasedViewerWidget()
		{
			ax::NodeEditor::DestroyEditor(nodeEditorCtxt);
		}

		/*!
		@brief Adds an ECellEngine::Editor::Utility::AssetNodeInfo in ::assetNodes.
		*/
		void AddAssetNode(const char* _name);

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