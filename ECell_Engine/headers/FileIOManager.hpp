#pragma once 

#include "SbmlParser.hpp"
#include "SbmlWriter.hpp"

class FileIOManager
{
private:
	SbmlWriter sbmlWriter;
	SbmlParser sbmlParser;

	std::string targetFilePath;

public:
	/*
	@brief Sets the @a activeDocument private member.
	*/
	inline void setTargetFilePath(std::string _filePath)
	{
		targetFilePath = _filePath;
	}

	/*
	@brief Opens & Process the file accessible at the directory
			/a targetFilePath.
	*/
	SBMLDocument* tryOpenTargetFileAsSBML();
};