#pragma once 

#include <filesystem> //for filesystem::path
#include <memory> //for std::shared_ptr

#include "DataState.hpp"
#include "Module.hpp"

namespace ECellEngine::IO
{
	/*
	@brief Base class for any asset importer.
	*/
	class ModuleImporter
	{
	public:
		~ModuleImporter() = default;

		/*!
		@brief Tries to parse a file.
		@param _filePath The system's path to the file.
		@param _dataState Reference to a datastate where additional data will be stored
				and containing other named variables that may be used during the import process.
		@returns If successful, an asset object encapsulated in a shared pointer. @a nullptr, otherwise.
		*/
		virtual const std::shared_ptr<ECellEngine::Data::Module> TryImport(const std::filesystem::path& _filePath, ECellEngine::Data::DataState& _dataState) noexcept = 0;
	};
}