#pragma once

#include "imgui.h"

#include "CommandsManager.hpp"

namespace ECellEngine::Editor
{
	class Widget
	{
	protected:
		CommandsManager* engineCmdsManager;

	public:

		Widget(CommandsManager* _cmdsManager) : engineCmdsManager(_cmdsManager)
		{

		}

		virtual void draw() = 0;

	};
}

