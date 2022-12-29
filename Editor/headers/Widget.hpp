#pragma once

#include "imgui.h"

#include "CommandsManager.hpp"

using namespace ECellEngine::IO;

namespace ECellEngine::Editor
{
	/*
	@brief Base class to write any widget of the editor.
	*/
	class Widget
	{
	protected:
		CommandsManager* engineCmdsManager;

	public:

		Widget(CommandsManager* _cmdsManager) : engineCmdsManager(_cmdsManager)
		{

		}

		/*
		@brief The method to call each frame to draw the content of the widget.
		*/
		virtual void Draw() = 0;

	};
}

