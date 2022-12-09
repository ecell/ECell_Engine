#include "ModuleImporterManager.hpp"

const std::shared_ptr<Module> ECellEngine::IO::ModuleImporterManager::TryImportModule(const std::filesystem::path& _filePath, const DataState* _dataState)
{
    auto importersIt = moduleImporters.equal_range(_filePath.extension().string());

    if (importersIt.first == moduleImporters.cend())
    {
        //Maybe log something like ("the file extension {_filePath.extension().string()} is not supported by the engine." in the engine console)
        std::cout << "The extension \"" + _filePath.extension().string() + "\" is not supported to load modules." << std::endl;
        return nullptr;
    }

    for (auto it = importersIt.first; it != importersIt.second; it++)
    {
        std::shared_ptr<Module> loadedModule = it->second->TryImport(_filePath, _dataState);

        if (loadedModule != nullptr)
            return loadedModule;
    }

    //If we reach this section, the import failed for some reason, log something or throw an error or something
    std::cout << "All importers supporting this extension failed to load the module." << std::endl;
    return nullptr;
}

//Instantiate fileImporters (in .cpp file)
ECellEngine::IO::SBMLModuleImporter ECellEngine::IO::ModuleImporterManager::sbmlModuleImporter;
const std::unordered_multimap<std::string, ECellEngine::IO::ModuleImporter*> ECellEngine::IO::ModuleImporterManager::moduleImporters = {
    { ".xml", &ECellEngine::IO::ModuleImporterManager::sbmlModuleImporter },
};
