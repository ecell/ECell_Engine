#pragma once

/*!
@file
@brief The source to store the data of the nodes to draw in a ECellEngine::Editor::ModelNodeBasedViewerWidget 
*/

#include <vector>

#include "Core/Simulation.hpp"

#include "Utility/NodeEditorDraw.hpp"
#include "Utility/NodeEditorStyle.hpp"

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
		@see ECellEngine::Editor::Utility::GetMNBVCtxtNextId()
		*/
		std::size_t uniqueId;

		/*!
		@brief The instance of styling data for the elements (nodes, pins, links)
				of this context.
		*/
		ECellEngine::Editor::Utility::NodeEditorStyle style;

#pragma region Nodes Lists
		/*!
		@brief The list of asset nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each asset imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::AssetNodeData> assetNodes;

		/*!
		@brief The list of computed parameter nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each computed parameter of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::ComputedParameterNodeData> computedParameterNodes;

		/*!
		@brief The list of nodes handling line plots in this context.
		@details It contains the information used to draw the nodes managing
				 line plots to display numeric data imported in the current simulation
				 space.
		*/
		std::vector<ECellEngine::Editor::Utility::LinePlotNodeData> linePlotNodes;

		/*!
		@brief The list of reaction nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each reaction of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::ReactionNodeData> reactionNodes;

		/*!
		@brief The list of simple parameter nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each simple parameter of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::SimpleParameterNodeData> simpleParameterNodes;
		
		/*!
		@brief The list of simulation time nodes.
		@details It contains the information used to draw the nodes corresponding
				 to display and use the elapsed time of the simulation.
		*/
		std::vector<ECellEngine::Editor::Utility::SimulationTimeNodeData> simulationTimeNodes;

		/*!
		@brief The list of solver nodes.
		@details It contains the information used to draw the nodes corresponding
				 to each solvers imported in the current simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::SolverNodeData> solverNodes;

		/*!
		@brief The list of species nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each species of various assets imported in the current
				 simulation space.
		*/
		std::vector<ECellEngine::Editor::Utility::SpeciesNodeData> speciesNodes;
		
		/*!
		@brief The list of value float nodes in this context.
		@details It contains the information used to draw the nodes to display, set
				and use custom float values.
		*/
		std::vector<ECellEngine::Editor::Utility::ValueFloatNodeData> valueFloatNodes;
#pragma endregion

		/*!
		@brief The list of links in this context.
		@details It contains the information used to draw the links between pins
				 of nodes in this context.
		*/
		std::vector<ECellEngine::Editor::Utility::LinkData> links;

		/*!
		@brief The matrix to encode whether a link can be created by the user
				between a specific pair of pins.
		@see ECellEngine::Editor::Utility::PinType.
		*/
		bool authorizedDynamicLinks[Utility::PinType_Count][Utility::PinType_Count]{};

		/*!
		@brief A local struct to encapsulate the parameters mandatory to send a
				a command to the engine to try to attach a solver to an asset.
		*/
		struct EngineTASToMCmdParameter
		{
			/*!
			@brief The name of the module we try to attach a solver to.
			*/
			const char* moduleName;

			/*!
			@brief The name of the solver we try to attach to a module.
			*/
			const char* solverName;

			EngineTASToMCmdParameter(const char* _moduleName, const char* _solverName) :
				moduleName{_moduleName}, solverName{ _solverName }
			{

			}
		};

		/*!
		@brief The number of queue commands parameters in ::TASToMCmds.
		*/
		unsigned short countTASToMCmds;

		/*!
		@brief The queue of parameters to be sent as part of commands to the
				engine core to try to attach a solver to a module.
		*/
		std::vector<EngineTASToMCmdParameter> TASToMCmds;

		ModelNodeBasedViewerContext() :
			uniqueId{ 0 }, countTASToMCmds{ 0 }
		{
			authorizedDynamicLinks[Utility::PinType_Solver][Utility::PinType_Solver] = true;
			authorizedDynamicLinks[Utility::PinType_ValueFloat][Utility::PinType_ValueFloat] = true;
		}

		/*!
		@brief The logic to draw the elements (nodes, pins, links) stored in
				this context.
		*/
		void Draw(ECellEngine::Core::Simulation* _simulation);
	};
}