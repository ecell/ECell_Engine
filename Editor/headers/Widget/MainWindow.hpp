#pragma once

#include "imgui_internal.h"
#include "Widget.hpp"

namespace ECellEngine::Editor::Widget
{
	/*!
	  @brief The widget that covers the whole background of the main window.
	  @details This widget is added to control the root docking space of the 
	 			editor as well as initialize the positions some other widgets.
	 			It also handles the menu item at the top of the window.
	*/
	class MainWindow : public Widget
	{
	private:

		/*!
		  @brief Flags for the main dock space fo the application.
		  @details Makes the central node transparent to avoid the default gray 
		 			background. We disable docking on the central node of the
		 			window.
		*/
		const ImGuiDockNodeFlags dockspaceFlags =
			ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode;

		
		/*!
		  @brief Flags for the main window that holds the main dockspace.
		  @details We make this window the background of the editor so we
		 			absolutely don't want it to move. We cannot interact with
		 			it and it has no title.
		*/
		const ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoDocking;

	public:
		MainWindow(Editor& _editor) :
			Widget(_editor)
		{

		}

		void Awake() override;

		void Draw() override;
	};
}