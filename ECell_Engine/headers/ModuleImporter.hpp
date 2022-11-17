#pragma once 

#include <filesystem> //for filesystem::path
#include <memory> //for std::shared_ptr

#include "Module.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::IO
{
	class ModuleImporter
	{
	public:
		~ModuleImporter() = default;

		virtual const std::shared_ptr<Module> TryImport(const std::filesystem::path& _filePath) noexcept = 0;
	};
}