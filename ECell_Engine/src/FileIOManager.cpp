#include "FileIOManager.hpp"

SBMLDocument* FileIOManager::tryOpenTargetFileAsSBML()
{
	std::cout << "Trying to open file: " << targetFilePath << std::endl;
	return sbmlParser.OpenSBMLFile((targetFilePath).c_str());
}