#pragma once

#include "imgui_internal.h"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class MainWindow : public Widget
	{
	public:
		MainWindow(Editor& _editor) :
			Widget(_editor)
		{

		}

		void Draw() override;
	};
}