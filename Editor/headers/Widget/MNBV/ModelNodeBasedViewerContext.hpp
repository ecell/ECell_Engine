#pragma once

/*!
@file
@brief The source to store the data of the nodes to draw in a ECellEngine::Editor::ModelNodeBasedViewerWidget 
*/

#include <vector>

#include "Core/IDProvider.hpp"
#include "Core/Simulation.hpp"
#include "Utility/MNBV/NodeEditorDraw.hpp"
#include "Style/NodeEditorStyle.hpp"

namespace ECellEngine::Editor::Widget::MNBV
{
	/*!
	@brief A struct to handle the data (nodes and links) to be displayed
			within a ECellEngine::Editor::ModelNodeBasedViewerWidget
	*/
	struct ModelNodeBasedViewerContext
	{
		/*!
		@brief The unique Id to be used by any newly created data that will be
				part of this context.
		@see ECellEngine::Editor::Widget::MNBV::GetMNBVCtxtNextId()
		*/
		ECellEngine::Core::IDProvider uniqueId;

#pragma region Nodes Lists

		/*!
		@brief The list of arithmetic operation nodes in this context.
		@details It contains the information used to draw the nodes to perform
				 arithmetic operation (addition, subtraction, multiplication,
				 division) based on couples of numbers.
		*/
		std::vector<Utility::MNBV::ArithmeticOperationNodeData> arithmeticOperationNodes;

		/*!
		@brief The list of asset nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each asset imported in the current simulation space.
		*/
		std::vector<Utility::MNBV::AssetNodeData> assetNodes;

		/*!
		@brief The list of equation nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each equation of various assets imported in the current
				 simulation space.
		*/
		std::vector<Utility::MNBV::EquationNodeData> equationNodes;

		/*!
		@brief The list of nodes handling line plots in this context.
		@details It contains the information used to draw the nodes managing
				 line plots to display numeric data imported in the current simulation
				 space.
		*/
		std::vector<Utility::MNBV::LinePlotNodeData> linePlotNodes;

		/*!
		@brief The list of logic operation nodes in this context.
		@details It contains the information used to draw the nodes to perform
				 logical operation (AND, OR, NOT) based on couples of booleans.
		*/
		std::vector<Utility::MNBV::LogicOperationNodeData> logicOperationNodes;

		/*!
		@brief The list of event nodes that can modify data state values in this
				context.
		@details It contains the information used to draw the nodes to display
				 the nodes that can modify data state values in this simulation
				 space.
		*/
		std::vector<Utility::MNBV::ModifyDataStateValueEventNodeData> modifyDataStateValueEventNodes;

		/*!
		@brief The list of reaction nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each reaction of various assets imported in the current
				 simulation space.
		*/
		std::vector<Utility::MNBV::ReactionNodeData> reactionNodes;

		/*!
		@brief The list of parameter nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each parameter of various assets imported in the current
				 simulation space.
		*/
		std::vector<Utility::MNBV::ParameterNodeData> parameterNodes;
		
		/*!
		@brief The list of time nodes.
		@details It contains the information used to draw the nodes corresponding
				 to display and use the various time information.
		*/
		std::vector<Utility::MNBV::TimeNodeData> timeNodes;

		/*!
		@brief The list of solver nodes.
		@details It contains the information used to draw the nodes corresponding
				 to each solvers imported in the current simulation space.
		*/
		std::vector<Utility::MNBV::SolverNodeData> solverNodes;

		/*!
		@brief The list of species nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each species of various assets imported in the current
				 simulation space.
		*/
		std::vector<Utility::MNBV::SpeciesNodeData> speciesNodes;
		
		/*!
		@brief The list of value float nodes in this context.
		@details It contains the information used to draw the nodes to display, set
				and use custom float values.
		*/
		std::vector<Utility::MNBV::ValueFloatNodeData> valueFloatNodes;

		/*!
		@brief The list of triggers in this context.
		@details It contains the information used to draw the nodes to display
				 the triggers of this simulation space.
		*/
		std::vector<Utility::MNBV::TriggerNodeData> triggerNodes;
#pragma endregion

#pragma region Links Data
		/*!
		@brief The list of dynamic links in this context.
		@details It contains the links that were created by the user to connect
				nodes as he needed.
		*/
		std::vector<Utility::MNBV::LinkData> dynamicLinks;

		/*!
		@brief The list of static links in this context.
		@details It contains the links that were created programatically alongside
				 nodes to represent the data relationships.
		*/
		std::vector<Utility::MNBV::LinkData> staticLinks;

		/*!
		@brief The matrix to encode whether a link can be created by the user
				between a specific pair of pins.
		@see Utility::MNBV::PinType.
		*/
		bool authorizedDynamicLinks[Utility::MNBV::PinType_Count][Utility::MNBV::PinType_Count]{};
#pragma endregion

		ModelNodeBasedViewerContext()
		{
			authorizedDynamicLinks[Utility::MNBV::PinType_Asset][Utility::MNBV::PinType_Asset] = true;
			
			//The interest of PinType_DataStateValueFloat might disappear in the future.
			authorizedDynamicLinks[Utility::MNBV::PinType_DataStateValueFloat][Utility::MNBV::PinType_FreeValueFloat] = true;
			authorizedDynamicLinks[Utility::MNBV::PinType_EquationValueFloat][Utility::MNBV::PinType_FreeValueFloat] = true;
			authorizedDynamicLinks[Utility::MNBV::PinType_FreeValueFloat][Utility::MNBV::PinType_FreeValueFloat] = true;

			authorizedDynamicLinks[Utility::MNBV::PinType_FreeValueFloat][Utility::MNBV::PinType_FloatCallBackSubscriber] = true;
			
			authorizedDynamicLinks[Utility::MNBV::PinType_BooleanCallBackPublisher][Utility::MNBV::PinType_BooleanCallBackSubscriber] = true;
			authorizedDynamicLinks[Utility::MNBV::PinType_FloatCallBackPublisher][Utility::MNBV::PinType_FloatCallBackSubscriber] = true;

			authorizedDynamicLinks[Utility::MNBV::PinType_Parameter][Utility::MNBV::PinType_Operand] = true;
			authorizedDynamicLinks[Utility::MNBV::PinType_Species][Utility::MNBV::PinType_Operand] = true;
		}

		/*!
		@brief Insures that the data pointers that may be alive between nodes of
				this context are reassigned to stay valid.
		@details Typically, when the size of the nodes lists are changed, the pointers
				 of nodes that are receiving data from other nodes will be invalidated.
				 This method is then called whenever a node is added.
		*/
		void ConserveLinkDataIntegrity();

		/*!
		@brief The logic to draw the elements (nodes, pins, links) stored in
				this context.
		*/
		void Draw(ECellEngine::Core::Simulation* _simulation);
	};
}