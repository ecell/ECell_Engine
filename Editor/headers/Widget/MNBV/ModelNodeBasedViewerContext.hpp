#pragma once

/*!
@file
@brief The source to store the data of the nodes to draw in a ECellEngine::Editor::ModelNodeBasedViewerWidget 
*/

#include <vector>

#include "Core/Simulation.hpp"

#include "Utility/MNBV/NodeEditorDraw.hpp"
#include "Utility/MNBV/NodeEditorStyle.hpp"

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
		std::size_t uniqueId;

		/*!
		@brief The instance of styling data for the elements (nodes, pins, links)
				of this context.
		*/
		Utility::MNBV::NodeEditorStyle style;

#pragma region Nodes Lists
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
		@brief The list of simulation time nodes.
		@details It contains the information used to draw the nodes corresponding
				 to display and use the elapsed time of the simulation.
		*/
		std::vector<Utility::MNBV::SimulationTimeNodeData> simulationTimeNodes;

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
		@brief The list of watchers in this context.
		@details It contains the information used to draw the nodes to display
				 the watchers of this simulation space.
		*/
		std::vector<Utility::MNBV::WatcherNodeData> watcherNodes;
#pragma endregion

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
			authorizedDynamicLinks[Utility::MNBV::PinType_Solver][Utility::MNBV::PinType_Solver] = true;
			authorizedDynamicLinks[Utility::MNBV::PinType_ValueFloat][Utility::MNBV::PinType_ValueFloat] = true;
		}

		/*!
		@brief The logic to draw the elements (nodes, pins, links) stored in
				this context.
		*/
		void Draw(ECellEngine::Core::Simulation* _simulation);
	};
}