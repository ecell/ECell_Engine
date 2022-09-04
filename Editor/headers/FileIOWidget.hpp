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
		std::vector<std::string> setFilePathCommandArray = { "setFilePath", "path"};
		const std::vector<std::string> addFileAsSBMLCommandArray = { "addFileAsSBML" };

	public:

		FileIOWidget(CommandsManager* _cmdsManager) :
			Widget(_cmdsManager)
		{

		}

		void draw() override;
	};
}
