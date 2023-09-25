#pragma once

/*!
@file
@brief The source to store the data of the nodes to draw in a ECellEngine::Editor::ModelNodeBasedViewerWidget 
*/

#include "Core/IDProvider.hpp"
#include "Core/Simulation.hpp"
#include "IO/CommandsManager.hpp"
#include "Util/BinarySearch.hpp"
#include "Utility/MNBV/NodeEditorDraw.hpp"

namespace ECellEngine::Editor::Widget::MNBV
{
	/*!
	@brief A struct to handle the data (nodes and links) to be displayed
			within a ECellEngine::Editor::ModelNodeBasedViewerWidget
	*/
	struct ModelNodeBasedViewerContext
	{
		/*!
		@brief The name of this context.
		*/
		char name[64] = { '0' };

		/*!
		@brief The unique Id to be used by any newly created data that will be
				part of this context.
		@see ECellEngine::Editor::Widget::MNBV::GetMNBVCtxtNextId()
		*/
		ECellEngine::Core::IDProvider idProvider;

		/*!
		@brief The buffer pointer to the simulation this context is associated to.
		@details It is used to access the data that can be displayed as nodes in the
				 ECellEngine::Editor::ModelNodeBasedViewerWidget. It is also used to
				 know which simulation commands should target.
		@remarks The value of this pointer should only change if a context moves from
				 one simulation to another. But be careful to check that the nodes
				 (and the data they encapsulate) is still relevant to the new simulation.
		*/
		Core::Simulation* simulation = nullptr;

		/*!
		@brief The pointer to the commands manager of the engine.
		@details It is used to get access to the commands defined in the engine
				 to be able to execute them.
		@remarks The value of this pointer should only change if a context has a need
				 for a different set of commands than the one defined in the engine.
		*/
		ECellEngine::IO::CommandsManager* engineCommandsManager = nullptr;

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
		@brief The list of parameter nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each parameter of various assets imported in the current
				 simulation space.
		*/
		std::vector<Utility::MNBV::ParameterNodeData> parameterNodes;
		
		/*!
		@brief The list of reaction nodes in this context.
		@details It contains the information used to draw the nodes corresponding
				 to each reaction of various assets imported in the current
				 simulation space.
		*/
		std::vector<Utility::MNBV::ReactionNodeData> reactionNodes;

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
		@brief The list of time nodes.
		@details It contains the information used to draw the nodes corresponding
				 to display and use the various time information.
		*/
		std::vector<Utility::MNBV::TimeNodeData> timeNodes;

		/*!
		@brief The list of triggers in this context.
		@details It contains the information used to draw the nodes to display
				 the triggers of this simulation space.
		*/
		std::vector<Utility::MNBV::TriggerNodeData> triggerNodes;

		/*!
		@brief The list of value float nodes in this context.
		@details It contains the information used to draw the nodes to display, set
				and use custom float values.
		*/
		std::vector<Utility::MNBV::ValueFloatNodeData> valueFloatNodes;
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
			Util::StrCopy(name, "New MNBV Context", sizeof(name));

			//increment immediately to work around default behaviour where ID 0 is not allowed.
			//see: https://github.com/thedmd/imgui-node-editor/issues/47
			++idProvider;

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
		@brief Gets the name of this context.
		@remark The name is public and can be accessed directly but we keep this
				method to be consistent with the other widgets and also to unify
				name access in several other places.
		*/
		inline char* GetName() noexcept
		{
			return name;
		}

		/*!
		@brief Sets the ::engineCommandsManager pointer of this context.
		*/
		inline void SetEngineCommandsManager(ECellEngine::IO::CommandsManager* _engineCommandsManager) noexcept
		{
			engineCommandsManager = _engineCommandsManager;
		}

		/*!
		@brief Sets the name of this context.
		@details Uses sprintf to copy the string @p _name in ::name.
				 Does not check whether the length of @p _name is less than 64.
		@param _name The new name of this context.
		*/
		inline void SetName(const char* _name) noexcept
		{
			Util::StrCopy(name, _name, sizeof(name));
		}

		/*!
		@brief Sets the ::simulation pointer of this context.
		*/
		inline void SetSimulation(Core::Simulation* _simulation) noexcept
		{
			simulation = _simulation;
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
		@brief Erases all the nodes of this context matching the given type by clearing
				the corresponding list.
		@param _nodeType The type of the nodes to erase. Allowed values are:
							- "Equation"
							- "Parameter"
							- "Reaction"
							- "Species"
							- "Arithmetic"
							- "Asset"
							- "LinePlot"
							- "LogicOperation"
							- "ModifyDataStateValueEvent"
							- "Solver"
							- "Time"
							- "Trigger"
							- "ValueFloat"
		@return @a True if the nodes were found (valid type) and erased, @a false otherwise.
		*/
		bool EraseAllNodesOfType(const char* _nodeType) noexcept;
		
		/*!
		@brief Erases the node of this context matching the given type and the
				given ID.
		@param _nodeType The type of the node to erase. Allowed values are:
							- "Equation"
							- "Parameter"
							- "Reaction"
							- "Species"
							- "Arithmetic"
							- "Asset"
							- "LinePlot"
							- "LogicOperation"
							- "ModifyDataStateValueEvent"
							- "Solver"
							- "Time"
							- "Trigger"
							- "ValueFloat"
		@param _nodeID The ID of the node to erase.
		@return @a True if the node was found (valid type and ID) and erased, @a false otherwise.
		*/
		bool EraseNodeOfType(const char* _nodeType, const std::size_t _nodeID) noexcept;

		/*!
		@brief Erases the node of with ID @p _nodeID in the list of nodes @p _nodes.
		@tparam NodeType The type of the node to erase.
		@param _nodes The list of nodes to erase from.
		@param _nodeID The ID of the node to erase.
		@return @a True if the node was found and erased, @a false otherwise.
		*/
		template<typename NodeType>
		bool EraseNodeOfType(std::vector<NodeType>& _nodes, const std::size_t _nodeID) noexcept
		{
			std::vector<NodeType>::iterator nodeIt = ECellEngine::Util::BinarySearch::LowerBound(_nodes.begin(), _nodes.end(), _nodeID);
			if (nodeIt != _nodes.end() && (std::size_t)nodeIt->id == _nodeID)
			{
				_nodes.erase(nodeIt);
				return true;
			}
			return false;
		}
	};
}