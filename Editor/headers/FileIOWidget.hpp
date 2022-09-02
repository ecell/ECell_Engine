#pragma once

#include "imgui.h"
#include "CommandsManager.hpp"
#include "FileIOManager.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class FileIOWidget : public Widget
	{
	private:
		FileIOManager* engineFileIOManager;

		const std::vector<std::string> addFileAsSBMLCommandArray = { "addFileAsSBML" };

	public:

		FileIOWidget(CommandsManager* _cmdsManager, FileIOManager* _fileIOManager) :
			Widget(_cmdsManager), engineFileIOManager(_fileIOManager)
		{

		}

		void draw() override;
	};
}
