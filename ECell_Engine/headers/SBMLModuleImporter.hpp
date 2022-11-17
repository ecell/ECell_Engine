#pragma once 

#include <filesystem>
#include <memory> //for shared_ptr
#include <unordered_map> //for unordered_multimap

#include <sbml/SBMLTypes.h>

#include "Module.hpp"
#include "ModuleImporter.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::IO
{
	class SBMLModuleImporter : public ModuleImporter
	{
	public:

		virtual const std::shared_ptr<Module> TryImport(const std::filesystem::path& _filePath) noexcept override;
	};
}