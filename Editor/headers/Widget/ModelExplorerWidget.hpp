#pragma once

/*!
@file
@brief The source for the base manager of everything related to visualization
		of a simulation in the editor.
*/

#include <string>
#include <vector>

#include "Logging/Logger.hpp"
#include "Widget/ModelHierarchyWidget.hpp"
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
		@brief 1-byte char to encode states of the widget
		@details Bit 0 encodes the open/close status of the popup to import an asset
				 from file.

				 Bit 1 encodes the open/close status of the popup to add a solver.

				 Bit 2 encodes the open/close status of the popup to set the preferences.
		*/
		unsigned char utilityState = 0;

		/*!
		@brief A char array to hold the path to an asset to import. 
		*/
		char assetPathBuffer[256] = "C:/Users/eliot/Documents/Source/External/Papers/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod2.xml";
		//char assetPathBuffer[256] = "C:/Users/eliot/Documents/Source/External/Papers/Novak-et-al_FissionYeast_CellCycle/BIOMD0000000111_L3V2.xml";
		
		/*!
		@brief A char array to hold the name of the asset node to be drawn in the
				editor and that matches the imported asset.
		*/
		char assetNameBuffer[64] = "p53_L3V2_mod2.xml";
		//char assetNameBuffer[64] = "FissionYeast_CellCycle_L3V2.xml";

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
		@brief The vector that contains the arguments for the 
				ECellEngine::IO::AddModuleCommand
		*/
		std::vector<std::string> addModuleCommandArray = {"addModule", "0", "path"};

		/*!
		@brief The vector that contains the arguments for the 
				ECellEngine::IO::AddSolverCommand
		*/
		std::vector<std::string> addSolverCommandArray = {"addSolver", "0", "solverName"};

		/*!
		@brief The widget to display the data added to the current simulation
				space as a tree.
		*/
		ModelHierarchyWidget modelHierarchy;
		
		/*!
		@brief The list of node editor contexts used in this Model Explorer.
		*/
		std::vector<ax::NodeEditor::EditorContext*> nodeEditorCtxts;

		/*!
		@brief The list of model node-based viewer contexts used in this
				Model Explorer.
		@see ECellEngine::Editor::Utility::ModelNodeBasedViewerContext
		*/
		std::vector<MNBV::ModelNodeBasedViewerContext> mnbvCtxts;

		/*!
		@brief The list of model viewers opened to explore the data added
				in the current simulation.
		*/
		std::vector<MNBV::ModelNodeBasedViewerWidget> mnbViewers;

		/*!
		@brief Draws the popup window used to inform the path of the asset to
				import in the current simulation space.
		*/
		void DrawImportAssetPopup();

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
		ModelExplorerWidget(Editor& _editor) :
			Widget(_editor), modelHierarchy(_editor)
		{
			mnbViewers.push_back(MNBV::ModelNodeBasedViewerWidget(editor, this));
		}

		/*!
		@brief Adds an editor context (@p _ctxt) to ::nodeEditorCtxts.
		@param _ctxt The pointer to a ax::NodeEditor::EditorContext to add.
		@remarks You probably also want to call ::AddNodeEditorContext()
		*/
		inline void AddNodeEditorContext(ax::NodeEditor::EditorContext* _ctxt)
		{
			nodeEditorCtxts.push_back(_ctxt);
		}

		/*!
		@brief Adds a new ECellEngine::Editor::Utility::ModelNodeBasedViewerContext() to
				::nodeEditorCtxts.
		*/
		inline void AddModelNodeBasedViewerContext()
		{
			mnbvCtxts.push_back(MNBV::ModelNodeBasedViewerContext());
		}

		/*!
		@brief Gets the size of ::nodeEditorCtxts
		*/
		inline std::size_t CountEditorContexts()
		{
			return nodeEditorCtxts.size();
		}

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
		inline ModelHierarchyWidget* GetModelHierarchy() noexcept
		{
			return &modelHierarchy;
		}

		/*!
		@brief Retrieves the pointer to the model node-based viewer at index
				@p _idx in ::mnbViewers.
		@param _idx The index of the viewer to retrieve in ::mnbViewers.
		*/
		inline MNBV::ModelNodeBasedViewerWidget* GetMNBViewer(const std::size_t _idx) noexcept
		{
			return &mnbViewers[_idx];
		}

		/*!
		@brief Retrieves the pointer to the node editor context at index
				@p _idx in ::nodeEditorCtxts.
		@param _idx The index of the context to retrieve in ::nodeEditorCtxts.
		*/
		inline ax::NodeEditor::EditorContext* GetNodeEditorContext(std::size_t _idx)
		{
			return nodeEditorCtxts[_idx];
		}

		/*!
		@brief Retrieves the pointer to the node editor style data at index
				@p _idx in ::mnbvCtxts.
		@param _idx The index of the style struct to retrieve in ::mnbvCtxts.
		*/
		inline MNBV::ModelNodeBasedViewerContext* GetModelNodeBasedViewerContext(std::size_t _idx)
		{
			return &mnbvCtxts[_idx];
		}

		/*!
		@brief Destroys and removes the pointer of the node editor context
				at index @p _idx in ::nodeEditorCtxts.
		@param _idx The index of the context to destroy and remove in
				::nodeEditorCtxts.
		@remarks Checks that @p _idx is not out of bounds of ::nodeEditorCtxts.
		*/
		inline void RemoveNodeEditorContext(std::size_t _idx)
		{
			if (_idx >= nodeEditorCtxts.size())
			{
				ECellEngine::Logging::Logger::LogError("Tried to destroy a NodeEditorContext at an index beyond the size of current list.");
				return;
			}

			ax::NodeEditor::DestroyEditor(nodeEditorCtxts[_idx]);
			nodeEditorCtxts.erase(nodeEditorCtxts.begin() + _idx);
		}

		/*!
		@brief Erases the node editor styles struct stored a index @p
				_idx in ::mnbvCtxts.
		@param _idx The index of the style struct to erase from
				::mnbvCtxts.
		@remarks Checks that @p _idx is not out of bounds of ::mnbvCtxts.
		*/
		inline void RemoveModelNodeBasedViewerContext(std::size_t _idx)
		{
			if (_idx >= mnbvCtxts.size())
			{
				ECellEngine::Logging::Logger::LogError("Tried to destroy a ModelNodeBasedViewerContext at an index beyond the size of current list.");
				return;
			}
			mnbvCtxts.erase(mnbvCtxts.begin() + _idx);
		}

		void Awake() override;

		void Draw() override;
	};
}