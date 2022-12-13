#pragma once 

#include <filesystem>
#include <memory> //for shared_ptr
#include <unordered_map> //for unordered_multimap

#include <sbml/SBMLTypes.h>

#include "Function.hpp"
#include "Module.hpp"
#include "ModuleImporter.hpp"
#include "SBMLModule.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::IO
{
	class SBMLModuleImporter : public ModuleImporter
	{
	private:
		const bool& ValidateSBML(SBMLDocument* _sbmlDocument);

		void InitializeParameters(SBMLModule* _sbmlModule, const Model* _model, std::unordered_map<std::string, std::shared_ptr<Operand>>& _idsToOperands);

		void InitializeReactions(SBMLModule* _sbmlModule, const Model* _model, std::unordered_map<std::string, std::shared_ptr<Operand>>& _idsToOperands);

		void InitializeSpecies(SBMLModule* _sbmlModule, const Model* _model, std::unordered_map<std::string, std::shared_ptr<Operand>>& _idsToOperands);

		Operation ASTNodeToOperation(const ASTNode* _node, const std::unordered_map<std::string, std::shared_ptr<Operand>>& _idsToOperands);

		std::shared_ptr<Operand> ASTNodeToOperand(const ASTNode* _node, const std::unordered_map<std::string, std::shared_ptr<Operand>>& _idsToOperands);

	public:
		virtual const std::shared_ptr<Module> TryImport(const std::filesystem::path& _filePath, DataState* _dataState) noexcept override;
	};
}