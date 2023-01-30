#pragma once 

#include <filesystem> //for filesystem::path
#include <memory> //for std::shared_ptr

#include "DataState.hpp"
#include "Module.hpp"

namespace ECellEngine::IO
{
	class ModuleImporter
	{
	public:
		~ModuleImporter() = default;

		virtual const std::shared_ptr<ECellEngine::Data::Module> TryImport(const std::filesystem::path& _filePath, ECellEngine::Data::DataState& _dataState) noexcept = 0;
	};
}