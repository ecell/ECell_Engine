#pragma once

#include "imgui_internal.h"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class MainWindow : public Widget
	{
	private:
		const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
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