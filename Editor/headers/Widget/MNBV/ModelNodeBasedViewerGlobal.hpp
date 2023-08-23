#pragma once

#include "Data/BinaryOperatedVector.hpp"
#include "IO/CommandsManager.hpp"
#include "Core/Simulation.hpp"
#include "Data/BiochemicalModule.hpp"
#include "Utility/MNBV/MNBVTypes.hpp"

namespace ECellEngine::Editor::Utility::MNBV
{
	//forward declaration
	struct NodePinData;
	struct NodeData;
	struct LinkData;
}

namespace ECellEngine::Editor::Widget::MNBV
{
	//forward declaration
	struct ModelNodeBasedViewerContext;

	/*!
	@file ModelNodeBasedViewerGlobal.hpp
	@brief Global API to access, query, add, remove, ... the nodes and links of
			ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext.
	@details Make sure to use ECellEngine::Editor::Widget::MNBV::SetCurrentMNBVContext(ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext* _ctxt)
			 before using these global functions.
	*/


	/*!
	@brief Adds an ECellEngine::Editor::Utility::MNBV::AssetNodeData in
			ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::assetNodes.
	@details After being added, the asset node data will be drawn in the editor.
	@param _data The data source of the asset node to add. It probably comes
			from a ECellEngine::Data::DataState.
	*/
	void AddAssetNode(ECellEngine::Data::Module* _data);

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
	@brief The logic to delete the visuals of a dynamic link (created by the user
			for data flow) and clean the data subscription.
	@param _dynamicLink The iterator of the link to erase in the list of dynamic links.
	*/
	void EraseDynamicLink(std::vector<Utility::MNBV::LinkData>::iterator& _dynamicLink);
	
	/*!
	@brief The logic to delete the visuals of a node.
	@param _nodeId The id of the link to erase.
	*/
	void EraseNode(const std::size_t _nodeId);

	/*!
	@brief The logic to delete the visuals of a static link (created to represent
			knowledge connection but not for data flow).
	@param _staticLink he iterator of the link to erase in the list of static links.
	*/
	void EraseStaticLink(std::vector<Utility::MNBV::LinkData>::iterator& _staticLink);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::MNBV::NodeData with id @p _id
			in all the nodes of the current ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext.
	@details Performs a binary search on each node list of the current
			ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext. It stops
			as soon as it found a match.
	@param _id The unique id of the target node.
	@returns A pointer to the ECellEngine::Editor::Utility::MNBV::NodeData with the
			 matching id if it was found. @a nullptr otherwise.
	@see ECellEngine::Editor::Widget::MNBV::FindNodeIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
	@see ECellEngine::Data::BinaryOperation::LowerBound(ForwardIt first, ForwardIt last, const T& value)
	*/
	Utility::MNBV::NodeData* FindNodeInAll(const std::size_t _id);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::MNBV::NodeData with id @p _id
			within the interval [@p _first, @p _last] of ECellEngine::Editor::Utility::MNBV::NodeData.
	@details Performs a binary search on the node data bounded by @p _first and
			@p _last (both included). The node data MUST be sorted in ascending
			order from @p _first to @p _last.
	@tparam ForwardIt A pointer (or iterator) to an object of type
			ECellEngine::Editor::Utility::MNBV::NodeData or deriving from it.
	@param _id The unique id of the target node.
	@param _first The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are searching.
	@param _last The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are searching.
	@returns A pointer to the ECellEngine::Editor::Utility::MNBV::NodeData with the
			 matching id if it was found. @a nullptr otherwise.
	@see ECellEngine::Editor::Widget::MNBV::FindNodeInAll(const std::size_t _id)
	*/
	template<class ForwardIt>
	Utility::MNBV::NodeData* FindNodeIn(const std::size_t _id, ForwardIt _first, ForwardIt _last);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::MNBV::NodePinData with id @p _id
			in all the nodes of the current ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext.
	@details Performs a binary search on each input and output node pin lists of
			each node list of the current ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext.
			This is possible because nodes ids are in ascending order with node pin
			ids. So, the strategy is to look for the lower nodeDataId that satisfies
			@p _id <= @a <tt> nodeData.id </tt>. Then, we know that the target node
			pin data is in the node with id <tt> nodeDataId-1 </tt> if it exists.
			The method stops as soon as it found a match.
	@param _id The unique id of the target node pin.
	@returns A pointer to the ECellEngine::Editor::Utility::MNBV::NodePinData with the
			 matching id if it was found. @a nullptr otherwise.
	@see ECellEngine::Editor::Widget::MNBV::FindNodePinIn(const std::size_t _id, ForwardIt _first, ForwardIt _last)
	@see ECellEngine::Data::BinaryOperation::LowerBound(ForwardIt first, ForwardIt last, const T& value)
	*/
	Utility::MNBV::NodePinData* FindNodePinInAll(const std::size_t _id);

	/*!
	@brief Searchs for a ECellEngine::Editor::Utility::MNBV::NodePinData with id @p _id
			within the interval [@p _first, @p _last] of ECellEngine::Editor::Utility::MNBV::NodeData.
	@details Performs a binary search on the node data bounded by @p _first and
			@p _last (both included). The node data MUST be sorted in ascending
			order from @p _first to @p _last.
	@tparam ForwardIt A pointer (or iterator) to an object of type
			ECellEngine::Editor::Utility::MNBV::NodeData or deriving from it.
	@param _id The unique id of the target node pin.
	@param _first The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are looking for the node pin.
	@param _last The pointer (or iterator) to the lowest bound of the sorted
			set of node data in which we are looking for the node pin.
	@returns A pointer to the ECellEngine::Editor::Utility::MNBV::NodePinData with the
			 matching id if it was found. @a nullptr otherwise.
	*/
	template<class ForwardIt>
	Utility::MNBV::NodePinData* FindNodePinIn(const std::size_t _id, ForwardIt _first, ForwardIt _last);

	/*!
	@brief Gets the pointer to the current active ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext.
	@see ECellEngine::Editor::Utility::MNBV::SetCurrentMNBVContext(ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext* _ctxt)
	*/
	ModelNodeBasedViewerContext* GetCurrentMNBVContext();

	/*!
	@brief Gets the pointer to the list of dynamic links (ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::dynamicLinks)
			of the current mnbv context.
	*/
	std::vector<Utility::MNBV::LinkData>& GetDynamicLinks();

	/*!
	@brief Gets the list of static links (ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::staticLinks)
			of the current mnbv context.
	*/
	std::vector<Utility::MNBV::LinkData>& GetStaticLinks();

	/*!
	@brief Gets the value of the next unique id of the current MNBV context.
	@details Effectively increments the member field ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::uniqueId
			 and returns the value.
	*/
	std::size_t& GetMNBVCtxtNextId();

	/*!
	@brief Checks whether a Link a user tries to make between two pin is authorized.
	@details Simply checks the value in the hand-built matrix
			 ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::authorizedDynamicLinks.
	@param _startPinType The type of pin that is used as the start of a
			potential ECellEngine::Editor::Utility::MNBV::LinkData.
	@param _endPinType The type of pin that is used as the end of a
			potential ECellEngine::Editor::Utility::MNBV::LinkData.
	*/
	bool IsDynamicLinkAuthorized(Utility::MNBV::PinType _startPinType, Utility::MNBV::PinType _endPinType);

	/*!
	@brief Queues a command to send to the core of the engine to try to attach
			the solver of id @p _solverID to the module of id @p _moduleID.
	@details In fact, it doesn't queue the command but an instance of
			ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::EngineTASToMCmdParameter
			which stores the parameters of the command. The queued commands are
			sent to the engine once the MNBV context has finished drawing the
			node and processing the users interactions of this frame.
	@see ECellEngine::Editor::Utility::MNBV::SendEngineTASToMCmd(const char* _simuIdx, ECellEngine::IO::CommandsManager* _cmdsManager)
	*/
	void QueueEngineTASToMCmd(const std::size_t _moduleID, const std::size_t _solverID);

	/*!
	@brief Removes as asset node from ModelNodeBasedViewerContext::assetNodes
	@param _idx The index of the node to erase from ModelNodeBasedViewerContext::assetNodes.
	*/
	void RemoveAssetNode(const std::size_t _idx);

	/*!
	@brief Sends the queue commands to the core of the engine to try to attach
			the solver of id @p _solverID to the module of id @p _moduleID.
	@see ECellEngine::Editor::Utility::MNBV::QueueEngineTASToMCmd(const std::size_t _moduleID, const std::size_t _solverID)
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

