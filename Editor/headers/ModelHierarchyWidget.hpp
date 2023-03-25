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

		/*!
		@brief The flags to transform normal tree nodes into leafs in the
				hierarchy.
		*/
		const ImGuiTreeNodeFlags leafNodeFlags =
			ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

		/*!
		@brief The vector of the assets' names displayed in the hierarchy of the
				model explorer.
		*/
		std::vector<char*> assetNames;

		/*!
		@brief The vector of the solvers' names displayed in the hierarchy of the
				model explorer.
		*/
		std::vector<char*> solverNames;

	public:
		ModelHierarchyWidget(Editor& _editor, ModelExplorerWidget* _rootExplorer) :
			Widget(_editor), rootExplorer{ _rootExplorer }
		{

		}

		/*!
		@brief Adds the name of an asset in ::assetNames
		*/
		inline void AddAssetName(char* _name)
		{
			assetNames.push_back(_name);
		}

		/*!
		@brief Adds the name of a solver in ::solverNames
		*/
		inline void AddSolverName(char* _name)
		{
			solverNames.push_back(_name);
		}

		/*!
		@brief Retrieves the assetName at index @p _idx.
		@param _idx The index of the name to retrieve in ::assetNames.
		*/
		inline char* GetAssetName(const std::size_t _idx) const noexcept
		{
			return assetNames[_idx];
		}

		/*!
		@brief Retrieves the solverName at index @p _idx.
		@param _idx The index of the name to retrieve in ::solverNames.
		*/
		inline char* GetSolverName(const std::size_t _idx) const noexcept
		{
			return solverNames[_idx];
		}

		inline void Awake() override {};

		void Draw() override;
	};

}