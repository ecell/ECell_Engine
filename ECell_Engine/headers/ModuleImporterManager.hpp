#pragma once 

#include <filesystem> //for filesystem::path
#include <iostream>
#include <memory> //for shared_ptr
#include <unordered_map> //for unordered_multimap

#include "Module.hpp"
#include "ModuleImporter.hpp"
#include "SBMLModuleImporter.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::IO
{
	class ModuleImporterManager
	{
	private:
		static SBMLModuleImporter sbmlModuleImporter;

		static const std::unordered_multimap<std::string, ModuleImporter*> moduleImporters;


	public:
		ModuleImporterManager() = default;

		const std::shared_ptr<Module> TryImportModule(const std::filesystem::path& _filePath) noexcept;
	};
}