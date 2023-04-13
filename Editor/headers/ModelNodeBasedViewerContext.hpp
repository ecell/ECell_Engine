#pragma once

#include <vector>

#include "DataState.hpp"

#include "NodeEditorDraw.hpp"
#include "NodeEditorStyle.hpp"

namespace ECellEngine::Editor::Utility
{
	struct ModelNodeBasedViewerContext
	{
		std::size_t uniqueId;

		ECellEngine::Editor::Utility::NodeEditorStyle style;

		/*!
		@brief The list of asset nodes in this viewer.
		@details It contains the information used to draw the nodes corresponding
				 to each asset imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::AssetNodeData> assetNodes;

		/*!
		@brief The list of computed parameter nodes in this viewer.
		@details It contains the information used to draw the nodes corresponding
				 to each computed parameter of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::ComputedParameterNodeData> computedParameterNodes;

		/*!
		@brief The list of reaction nodes in this viewer.
		@details It contains the information used to draw the nodes corresponding
				 to each reaction of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::ReactionNodeData> reactionNodes;

		/*!
		@brief The list of simple parameter nodes in this viewer.
		@details It contains the information used to draw the nodes corresponding
				 to each simple parameter of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::SimpleParameterNodeData> simpleParameterNodes;

		/*!
		@brief The list of species nodes in this viewer.
		@details It contains the information used to draw the nodes corresponding
				 to each species of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::SpeciesNodeData> speciesNodes;

		/*!
		@brief The list of solver nodes.
		@details It contains the information used to draw the nodes corresponding
				 to each solvers imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::SolverNodeData> solverNodes;

		std::vector<ECellEngine::Editor::Utility::LinkData> links;// List of live links. It is dynamic unless you want to create read-only view over nodes.

		//bool validDynamicPinLinks[PinType_Count][PinType_Count];

		ModelNodeBasedViewerContext() :
			uniqueId{ 0 }
		{

		}

		void Draw(ECellEngine::Data::DataState* _dataState);
	};
}