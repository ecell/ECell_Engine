#pragma once

#include <string>
#include <vector>

#include "DataState.hpp"
#include "Logger.hpp"
#include "ModelHierarchyWidget.hpp"
#include "ModelNodeBasedViewerWidget.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	/*!
	@brief The window that contains the GUI to explore the assembled model of a
			simulation.
	*/
	class ModelExplorerWidget : public Widget
	{
	private:

		/*!
		@brief Pointer to the data state of the simulation space visualized in
				this model explorer.
		*/
		ECellEngine::Data::DataState* dataState;

		/*!
		@brief 1-byte char to encode states of the widget
		@details Bit 0 encodes the open/close status of the popup to import an asset
				 from file.

				 Bit 1 encodes the open/close status of the popup to add a solver.
		*/
		unsigned char utilityState = 0;

		/*!
		@brief A char array to hold the path to an asset to import. 
		*/
		char assetPathBuffer[256] = "C:/Users/eliot/Documents/Source/External/Papers/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod2.xml";
		
		/*!
		@brief A char array to hold the name of the asset node to be drawn in the
				editor and that matches the imported asset.
		*/
		char assetNameBuffer[256] = "p53_L3V2_mod2.xml";

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
		@brief The list of model viewers opened to explore the data added
				in the current simulation.
		*/
		std::vector<ModelNodeBasedViewerWidget> mnbViewers;

		/*!
		@brief Draws the popup window used to select which solver to add in the
				current simulation space.
		*/
		void DrawAddSolverPopup();

		/*!
		@brief Draws the popup window used to inform the path of the asset to
				import in the current simulation space.
		*/
		void DrawImportAssetPopup();

		/*!
		@brief Draw every options of the Model Explorer's menu bar.
		*/
		void DrawMenuBar();
		

	public:
		ModelExplorerWidget(Editor& _editor) :
			Widget(_editor), modelHierarchy(_editor, this)
		{
			mnbViewers.push_back(ModelNodeBasedViewerWidget(editor, this));
		}

		/*!
		@brief Get the pointer of the data state visualized in this model
				explorer.
		*/
		inline ECellEngine::Data::DataState* GetDataState() noexcept
		{
			return dataState;
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
		inline ModelNodeBasedViewerWidget* GetMNBViewer(const std::size_t _idx) noexcept
		{
			return &mnbViewers[_idx];
		}

		/*!
		@brief Set the pointer of the data state to be visualized in this
				model explorer.
		@remarks Needs an access to ::editor which class is forward declared
				 so we cannot inline this function and it must be defined in
				 the .cpp. 
		*/
		void SetDataState(std::size_t _simuIdx);

		void Awake() override;

		void Draw() override;
	};
}