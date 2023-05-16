#pragma once

#include "imgui.h"
#include "CommandsManager.hpp"
#include "FileIOManager.hpp"
#include "Widget.hpp"

using namespace ECellEngine::IO;

namespace ECellEngine::Editor::Widget
{
	class FileIOWidget : public Widget
	{
	private:
		std::vector<SBMLDocument*>* loadedSBMLDocuments; // A pointer to the member of same name in Engine
		short nbLoadedSBMLDocuments = 0; // A local copy of the number of loaded SBMLDocs to avoid using loadedSBMLDocuments->size() all the time.
		std::vector<std::string> loadedSBMLDocumentNames; // A vector of the names of SBMLDocs to avoid using the pointer to Engine all the time		

		std::vector<std::string> setFilePathCommandArray = { "setFilePath", "path"};
		const std::vector<std::string> addFileAsSBMLCommandArray = { "addFileAsSBML" };
		std::vector<std::string> addSimulationTargetCommandArray = { "addSimulationTarget", "idx"};

		void drawLoadedSBMLDocuments();
		void drawSearchTargetFile();

	public:

		FileIOWidget(CommandsManager* _cmdsManager, std::vector<SBMLDocument*>* _loadedSBMLDocuments) :
			Widget(_cmdsManager), loadedSBMLDocuments(_loadedSBMLDocuments)
		{

		}

		void draw() override;
	};
}
