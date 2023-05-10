#pragma once

#include "BinaryOperatedVector.hpp"
#include "CommandsManager.hpp"
#include "Simulation.hpp"
#include "SBMLModule.hpp"
#include "NodeEditorStyle.hpp"

namespace ECellEngine::Editor::Utility
{
	//forward declaration
	struct ModelNodeBasedViewerContext;
	struct NodePinData;
	struct NodeData;
	struct LinkData;
	
	/*!
	@file ModelNodeBasedViewerGlobal.hpp
	@brief Global API to access, query, add, remove, ... the nodes and links of
			ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
	@details Make sure to use ECellEngine::Editor::Utility::SetCurrentMNBVContext(ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* _ctxt)
			 before using these global functions.
	*/


	/*!
	@brief Adds an ECellEngine::Editor::Utility::AssetNodeData in
			ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::assetNodes.
	@details After being added, the asset node data will be drawn in the editor.
	@param _data The data source of the asset node to add. It probably comes
			from a ECellEngine::Data::DataState.
	*/
	void AddAssetNode(ECellEngine::Data::Module* _data);
	
	/*!
	@brief Adds an ECellEngine::Editor::Utility::SolverNodeData in
			ModelNodeBasedViewerContext::solverNodes.
	@details After being added, the asset node data will be drawn in the editor.
	@param _data The data source of the asset node to add. It probably comes
			from a ECellEngine::Data::DataState.
	*/
	void AddSolverNode(ECellEngine::Solvers::Solver* _data);

	/*!
	@brief Asks the current Model Node Based Viewer (MNBV) Context to draw the
			nodes registered to it.
	@param  _simulation The pointer to the simulation from where the nodes
			registered in the current MNBV come from. This is used to keep
			adding nodes to the MNBV context depending on the user's
			interaction with the node editor.
	*/
	void CurrentMNBVContextDraw(ECellEngine::Core::Simulation* _simulation);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::NodeData with id @p _id
			in all the nodes of the current ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
	@details Performs a binary search on each node list of the current
			ECellEngine::Editor::Utility::ModelNodeBasedViewerContext. It stops
			as soon as it found a match.
	@param _id The unique id of the target node.
	@returns A pointer to the ECellEngine::Editor::Utility::NodeData with the
			 matching id if it was found. @a nullptr otherwise.
	@see ECellEngine::Editor::Utility::FindNodeIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
	@see ECellEngine::Data::BinaryOperation::LowerBound(ForwardIt first, ForwardIt last, const T& value)
	*/
	NodeData* FindNodeInAll(const std::size_t _id);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::NodeData with id @p _id
			within the interval [@p _first, @p _last] of ECellEngine::Editor::Utility::NodeData.
	@details Performs a binary search on the node data bounded by @p _first and
			@p _last (both included). The node data MUST be sorted in ascending
			order from @p _first to @p _last.
	@tparam ForwardIt A pointer (or iterator) to an object of type
			ECellEngine::Editor::Utility::NodeData or deriving from it.
	@param _id The unique id of the target node.
	@param _first The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are searching.
	@param _last The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are searching.
	@returns A pointer to the ECellEngine::Editor::Utility::NodeData with the
			 matching id if it was found. @a nullptr otherwise.
	@see ECellEngine::Editor::Utility::FindNodeInAll(const std::size_t _id)
	*/
	template<class ForwardIt>
	NodeData* FindNodeIn(const std::size_t _id, ForwardIt _first, ForwardIt _last);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::NodePinData with id @p _id
			in all the nodes of the current ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
	@details Performs a binary search on each input and output node pin lists of
			each node list of the current ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
			This is possible because nodes ids are in ascending order with node pin
			ids. So, the strategy is to look for the lower nodeDataId that satisfies
			@p _id <= @a <tt> nodeData.id </tt>. Then, we know that the target node
			pin data is in the node with id <tt> nodeDataId-1 </tt> if it exists.
			The method stops as soon as it found a match.
	@param _id The unique id of the target node pin.
	@returns A pointer to the ECellEngine::Editor::Utility::NodePinData with the
			 matching id if it was found. @a nullptr otherwise.
	@see ECellEngine::Editor::Utility::FindNodePinIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
	@see ECellEngine::Data::BinaryOperation::LowerBound(ForwardIt first, ForwardIt last, const T& value)
	*/
	NodePinData* FindNodePinInAll(const std::size_t _id);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::NodePinData with id @p _id
			within the interval [@p _first, @p _last] of ECellEngine::Editor::Utility::NodeData.
	@details Performs a binary search on the node data bounded by @p _first and
			@p _last (both included). The node data MUST be sorted in ascending
			order from @p _first to @p _last.
	@tparam ForwardIt A pointer (or iterator) to an object of type
			ECellEngine::Editor::Utility::NodeData or deriving from it.
	@param _id The unique id of the target node pin.
	@param _first The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are looking for the node pin.
	@param _last The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are looking for the node pin.
	@returns A pointer to the ECellEngine::Editor::Utility::NodePinData with the
			 matching id if it was found. @a nullptr otherwise.
	*/
	template<class ForwardIt>
	NodePinData* FindNodePinIn(const std::size_t _id, ForwardIt _first, ForwardIt _last);

	/*!
	@brief Gets the pointer to the current active ECellEngine::Editor::Utility::ModelNodeBasedViewerContext.
	@see ECellEngine::Editor::Utility::SetCurrentMNBVContext(ECellEngine::Editor::Utility::ModelNodeBasedViewerContext* _ctxt)
	*/
	ModelNodeBasedViewerContext* GetCurrentMNBVContext();

	/*!
	@brief Gets the pointer to the list of the links (ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::links)
			of the current mnbv context.
	*/
	std::vector<LinkData>* GetLinks();

	/*!
	@brief Gets the pointer to the member field ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::style
			of the current active MNBV context.
	*/
	NodeEditorStyle* GetMNBVStyle();

	/*!
	@brief Gets the value of the next unique id of the current MNBV context.
	@details Effectively increments the member field ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::uniqueId
			 and returns the value.
	*/
	std::size_t& GetMNBVCtxtNextId();

	/*!
	@brief Gets the pointer to the array of colors used to cutomize the aspect of a
			node of type @p _nodeType.
	@param _nodeType The type of node of which we want to retrieve the color set.
	@remarks A reminder that in c++ the pointer to the first element of an array
			 is the pointer to the array itself.
	@see ECellEngine::Editor::Utility::NodeEditorStyle::nodeColors
	*/
	ImVec4* GetNodeColors(NodeType _nodeType);

	/*!
	@brief Translates a value of the enum ECellEngine::Editor::Utility::NodeColorType
			to a character string.
	@param _nodeColorType The type of node color for which we want to retrieve
			the name.
	*/
	inline char* GetNodeColorTypeName(NodeColorType _nodeColorType)
	{
		switch (_nodeColorType)
		{
		case NodeColorType_Bg:
			return "NodeColorType_Bg";
		case NodeColorType_Border:
			return "NodeColorType_Border";
			/*case NodeColorType_BorderHovered:
				return "NodeColorType_BorderHovered";*/
				/*case NodeColorType_BorderSelected:
					return "NodeColorType_BorderSelected";*/
		case NodeColorType_HeaderBg:
			return "NodeColorType_HeaderBg";
		case NodeColorType_HeaderActivated:
			return "NodeColorType_HeaderActivated";
		case NodeColorType_HeaderHovered:
			return "NodeColorType_HeaderHovered";
		default:
			return "Unknown NodeColorType";
		}
	}

	/*!
	@brief Translates a value of the enum ECellEngine::Editor::Utility::NodeType
			to a character string.
	@param _nodeType The type of node for which we want to retrieve the name.
	*/
	inline char* GetNodeTypeName(NodeType _nodeType)
	{
		switch (_nodeType)
		{
		case NodeType_Default:
			return "NodeType_Default";
		case NodeType_Asset:
			return "NodeType_Asset";
		case NodeType_Data:
			return "NodeType_Data";
		case NodeType_Parameter:
			return "NodeType_Parameter";
		case NodeType_Reaction:
			return "NodeType_Reaction";
		case NodeType_Solver:
			return "NodeType_Solver";
		case NodeType_Species:
			return "NodeType_Species";
		default:
			return "Unknown NodeType";
		}
	}

	/*!
	@brief Gets the pointer to the array of colors used to cutomize the aspect of a
			pin of type @p _pinType.
	@param _pinType The type of pin for which we want to retrieve the color set.
	@remarks A reminder that in c++ the pointer to the first element of an array
			 is the pointer to the array itself.
	@see ECellEngine::Editor::Utility::NodeEditorStyle::pinColors
	*/
	ImVec4* GetPinColors(PinType _pinType);

	/*!
	@brief Translates a value of the enum ECellEngine::Editor::Utility::PinColorType
			to a character string.
	@param _pinColorType The type of node pin color for which we want to retrieve
			the name.
	*/
	inline char* GetPinColorTypeName(PinColorType _pinColorType)
	{
		switch (_pinColorType)
		{
		case PinColorType_BgActivated:
			return "PinColorType_BgActivated";
		case PinColorType_BgInactivated:
			return "PinColorType_BgInactivated";
		case PinColorType_Border:
			return "PinColorType_Border";
		default:
			return "Unknown PinColorType";
		}
	}

	/*!
	@brief Translates a value of the enum ECellEngine::Editor::Utility::PinType
			to a character string.
	@param _pinType The type of pin for which we want to retrieve the name.
	*/
	inline char* GetPinTypeName(PinType _pinType)
	{
		switch (_pinType)
		{
		case PinType_Default:
			return "PinType_Default";
		case PinType_Asset:
			return "PinType_Asset";
		case PinType_Parameter:
			return "PinType_Parameter";
		case PinType_Reaction:
			return "PinType_Reaction";
		case PinType_Solver:
			return "PinType_Solver";
		case PinType_Species:
			return "PinType_Species";
		case PinType_ValueFloat:
			return "PinType_ValueFloat";
		default:
			return "Unknown PinType";
		}
	}

	/*!
	@brief Checks whether a Link a user tries to make between two pin is authorized.
	@details Simply checks the value in the hand-built matrix
			 ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::authorizedDynamicLinks.
	@param _startPinType The type of pin that is used as the start of a
			potential ECellEngine::Editor::Utility::LinkData.
	@param _endPinType The type of pin that is used as the end of a
			potential ECellEngine::Editor::Utility::LinkData.
	*/
	bool IsDynamicLinkAuthorized(PinType _startPinType, PinType _endPinType);

	/*!
	@brief Queues a command to send to the core of the engine to try to attach
			the solver of name @p _solverName to the module of name @p _moduleName.
	@details In fact, it doesn't queue the command but an instance of 
			ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::EngineTASToMCmdParameter
			which stores the parameters of the command. The queued commands are
			sent to the engine once the MNBV context has finished drawing the
			node and processing the users interactions of this frame.
	@see ECellEngine::Editor::Utility::SendEngineTASToMCmd(const char* _simuIdx, ECellEngine::IO::CommandsManager* _cmdsManager)
	*/
	void QueueEngineTASToMCmd(const char* _moduleName, const char* _solverName);

	/*!
	@brief Removes as asset node from ModelNodeBasedViewerContext::assetNodes
	@param _idx The index of the node to erase from ModelNodeBasedViewerContext::assetNodes.
	*/
	void RemoveAssetNode(const std::size_t _idx);

	/*!
	@brief Sends the queue commands to the core of the engine to try to attach
			the solver of name @p _solverName to the module of name @p _moduleName.
	@see ECellEngine::Editor::Utility::QueueEngineTASToMCmd(const char* _moduleName, const char* _solverName)
	*/
	void SendEngineTASToMCmd(const char* _simuIdx, CommandsManager* _cmdsManager);

	/*!
	@brief Sets the current context.
	@details Every call to global functions in this file will take this context as
			 reference. You tipically set the current context at the beginning
			 of every drawing loop of a particular ECellEngine::Editor::ModelNodeBasedViewerWidget.
	*/
	void SetCurrentMNBVContext(ModelNodeBasedViewerContext* _ctxt);
}

