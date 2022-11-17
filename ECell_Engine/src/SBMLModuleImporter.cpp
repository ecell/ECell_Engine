#include "SBMLModuleImporter.hpp"

const std::shared_ptr<Module> ECellEngine::IO::SBMLModuleImporter::TryImport(const std::filesystem::path& _filePath)
{
	// use the Sbml importer API to check if the file is okay
	// if so, then parse the file, build an SBMLModule and return it as a pointer
	// if not return a null_ptr
}