#pragma once 

#include <filesystem> //for filesystem::path
#include <memory> //for shared_ptr
#include <unordered_map> //for unordered_multimap

#include "Data/DataState.hpp"
#include "Logger.hpp"
#include "Data/Module.hpp"
#include "IO/ModuleImporter.hpp"
#include "IO/SBMLModuleImporter.hpp"

namespace ECellEngine::IO
{
	class ModuleImporterManager
	{
	private:
		static SBMLModuleImporter sbmlModuleImporter;

		static const std::unordered_multimap<std::string, ModuleImporter*> moduleImporters;


	public:
		ModuleImporterManager() = default;

		const std::shared_ptr<ECellEngine::Data::Module> TryImportModule(const std::filesystem::path& _filePath, ECellEngine::Data::DataState& _dataState) noexcept;
	};
}