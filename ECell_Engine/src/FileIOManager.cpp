#include "FileIOManager.hpp"

SBMLDocument* ECellEngine::IO::FileIOManager::tryOpenTargetFileAsSBML()
{
	std::cout << "Trying to open file: " << targetFilePath << std::endl;
	return sbmlParser.OpenSBMLFile((targetFilePath).c_str());
}