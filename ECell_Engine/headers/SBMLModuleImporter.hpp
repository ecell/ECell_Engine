#pragma once 

#include <filesystem>
#include <unordered_map> //for unordered_multimap

#include <sbml/SBMLTypes.h>

#include "Constant.hpp"
#include "Function.hpp"
#include "ModuleImporter.hpp"
#include "SBMLModule.hpp"

namespace ECellEngine::IO
{
	class SBMLModuleImporter : public ModuleImporter
	{
	private:
		const bool ValidateSBML(SBMLDocument* _sbmlDocument);

		void InitializeParameters(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

		void InitializeReactions(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

		void InitializeSpecies(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

		Operation* ASTNodeToOperation(ECellEngine::Data::DataState& _dataState, const std::string& _rootName, const ASTNode* _node, const std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

	public:
		virtual const std::shared_ptr<ECellEngine::Data::Module> TryImport(const std::filesystem::path& _filePath, ECellEngine::Data::DataState& _dataState) noexcept override;
	};
}