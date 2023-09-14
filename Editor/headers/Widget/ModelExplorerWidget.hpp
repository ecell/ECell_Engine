#pragma once

/*!
@file
@brief The source for the base manager of everything related to visualization
		of a simulation in the editor.
*/

#include <string>
#include <vector>

#include "Widget/ModelHierarchyWidget.hpp"
#include "Widget/SimulationFlowControlWidget.hpp"
#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"
#include "Widget/MNBV/ModelNodeBasedViewerWidget.hpp"

namespace ECellEngine::Editor::Widget
{
	/*!
	@brief The window that contains the GUI to explore the assembled model of a
			simulation.
	*/
	class ModelExplorerWidget : public Widget
	{
	private:

		/*!
		@brief The comparator used to sort the contexts in ::mnbvCtxts by the id
				of the simulation they are based on.
		*/
		struct CompareMNBVCtxtBySimulation
		{
			bool operator()(const MNBV::ModelNodeBasedViewerContext& _lhs,
							const MNBV::ModelNodeBasedViewerContext& _rhs) const
			{
				return _lhs.simulation->id < _rhs.simulation->id;
			}

			bool operator()(const MNBV::ModelNodeBasedViewerContext& _lhs,
							const std::size_t _id) const
			{
				return _lhs.simulation->id < _id;
			}
		};

	private:
		/*!
		@brief 1-byte char to encode states of the widget
		@details Bit 0 encodes the open/close status of the popup to import an asset
				 from file.

				 Bit 1 encodes the open/close status of the popup to add a solver.

				 Bit 2 encodes the open/close status of the popup to set the preferences.
		*/
		unsigned char utilityState = 0;

		/*!
		  @brief Flags for the model explorer window.
		  @details Enables the MenuBar.
		*/
		const ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_MenuBar;

		/*!
		@brief Flags for the custom popup windows spawned from this widget.
		*/
		const ImGuiWindowFlags popupWindowFlags =
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;

		/*!
		@brief The widget to display the data added to the current simulation
				space as a tree.
		*/
		ModelHierarchyWidget modelHierarchy;

		/*!
		@brief The list of model node-based viewer contexts used in this
				Model Explorer.
		@details The contexts are sorted by the id of the simulation they are
				 based on.
		@remark There is always at least one context in this vector. In particular
				::EraseModelNodeBasedViewerContext(std::size_t _idx) will never
				erase the last context.
		@see ECellEngine::Editor::Utility::ModelNodeBasedViewerContext
		*/
		std::vector<MNBV::ModelNodeBasedViewerContext> mnbvCtxts;

		/*!
		@brief The list of model viewers opened to explore the data added
				in the current simulation.
		*/
		std::vector<MNBV::ModelNodeBasedViewerWidget> mnbViewers;

		/*!
		@brief The list of the indeces of the contexts that are currently
				being displayed in the viewers.
		@details The size of this vector is equal to the size of ::mnbViewers.
				 The value at index i is the index of the context in ::mnbvCtxts
				 displayed by the viewer at index i in ::mnbViewers.
		*/
		std::vector<unsigned short> ctxtsPerViewer;

		/*!
		@brief The widget to control the flow of time of the simulation.
		*/
		SimulationFlowControlWidget simuFlowControl;

		/*!
		@brief Draws the popup window used to consult and change the preference
				values related to the Model Explorer.
		*/
		void DrawPreferencesPopup();

		/*!
		@brief Draw every options of the Model Explorer's menu bar.
		*/
		void DrawMenuBar();

		/*!
		@brief Utility function to switch the value 0 -> 1 or 1 -> 0 of the bit
				at position @p _stateBitPos in ::utilityState.
		@remarks It could be moved outside of this class in the future since the 
				 code works with any data and is not pecific to this context.
		*/
		inline void SwitchState(const short _stateBitPos)
		{
			utilityState ^= (1 << _stateBitPos);
		}
		

	public:
		ModelExplorerWidget(Editor* _editor) :
			Widget(_editor), modelHierarchy(_editor), simuFlowControl(_editor)
		{
			//By default when opening a new explorer widget:
			
			// - We add one context and one viewer. The context is added to first simulation.
			mnbvCtxts.emplace_back(MNBV::ModelNodeBasedViewerContext());
			mnbViewers.emplace_back(MNBV::ModelNodeBasedViewerWidget(editor));

			// - The viewer will display the context at index 0.
			ctxtsPerViewer.emplace_back(0);
			modelHierarchy.SetMNBVCtxts(&mnbvCtxts);

		}

		/*!
		@brief Adds a new ECellEngine::Editor::Utility::ModelNodeBasedViewer() to
				::mnbViewers.
		*/
		void AddModelNodeBasedViewerWidget();

		/*!
		@brief Adds a new ECellEngine::Editor::Utility::ModelNodeBasedViewerContext to
				::mnbvCtxts.
		@param _simulation The simulation the new context will be based on.
		@see ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::simulation
		*/
		void AddModelNodeBasedViewerContext(Core::Simulation* _simulation);

		/*!
		@brief Gets the size of ::mnbvCtxts
		*/
		inline std::size_t CountModelNodeBasedViewerContext()
		{
			return mnbvCtxts.size();
		}

		/*!
		@brief Retrieves the pointer to ::modelHierarchy
		*/
		/*inline ModelHierarchyWidget& GetModelHierarchy() noexcept
		{
			return modelHierarchy;
		}*/

		/*!
		@brief Retrieves the pointer to the model node-based viewer at index
				@p _idx in ::mnbViewers.
		@param _idx The index of the viewer to retrieve in ::mnbViewers.
		*/
		/*inline MNBV::ModelNodeBasedViewerWidget& GetMNBViewer(const std::size_t _idx) noexcept
		{
			return mnbViewers[_idx];
		}*/

		/*!
		@brief Retrieves the pointer to the node editor style data at index
				@p _idx in ::mnbvCtxts.
		@param _idx The index of the style struct to retrieve in ::mnbvCtxts.
		*/
		/*inline MNBV::ModelNodeBasedViewerContext& GetModelNodeBasedViewerContext(std::size_t _idx)
		{
			return mnbvCtxts[_idx];
		}*/

		/*!
		@brief Erases the model node-based viewer at index @p _idx in ::mnbViewers.
		@param _idx The index of the viewer to erase from ::mnbViewers.
		@remarks Checks that @p _idx is not out of bounds of ::mnbViewers.
		*/
		bool EraseModelNodeBasedViewerWidget(std::size_t _idx);

		/*!
		@brief Erases the node editor styles struct stored a index @p _idx in
				::mnbvCtxts.
		@param _idx The index of the style struct to erase from ::mnbvCtxts.
		@remarks Checks that @p _idx is not out of bounds of ::mnbvCtxts. Does
				 not erase the last context.
		*/
		bool EraseModelNodeBasedViewerContext(std::size_t _idx);

		/*!
		@brief Focus on the node with ID @p _nodeID in every possible viewer.
				Focusing means the node will be selected and the viewer will
				center on it.
		@details Linear search on ::ctxtsPerViewer. Loops through the
				::ctxtsPerViewer and tries to focus on a node with ID @p _nodeID
				for all viewers that use the context with index @p _contextIdx.
		@remark Does not check whether the context at index @p _contextIdx in
				::mnbvCtxts is currently being displayed in a viewer. See 
				::IsMNBVContextInUse(std::size_t _idx) for that.
		@param _contextIdx The index of the context in ::mnbvCtxts to use.
		@param _nodeID The ID of the node to focus on.
		@returns @a True if the node was found and focused on. @a False otherwise.
		*/
		bool FocusNode(const unsigned short _contextIdx, const unsigned long long _nodeID);

		/*!
		@brief Checks whether the context at index @p _idx in ::mnbvCtxts is
				currently being displayed in a viewer.
		@details Linear search on ::ctxtsPerViewer. Loops through the
				::ctxtsPerViewer and tests <em> _contextIdx == ctxtsPerViewer[i] </em>.
		@returns A pair containing a boolean indicating whether the context is
				 currently being displayed in a viewer and an iterator to the
				 index of the viewer in ::ctxtsPerViewer. If the context is not
				 in use the boolean is false and the iterator is ::ctxtsPerViewer.end().
		*/
		std::pair<bool, std::vector<unsigned short>::iterator> IsMNBVContextInUse(std::size_t _idx);

		void Awake() override;

		void Draw() override;
	};
}