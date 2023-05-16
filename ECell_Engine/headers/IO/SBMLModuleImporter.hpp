#pragma once 

#include <filesystem> //for path
#include <unordered_map> //for unordered_multimap

#include <sbml/SBMLTypes.h>

#include "Maths/Function.hpp"
#include "IO/ModuleImporter.hpp"
#include "Data/SBMLModule.hpp"

namespace ECellEngine::IO
{
	/*!
	@brief The importer specialized for SBML files.
	*/
	class SBMLModuleImporter : public ModuleImporter
	{
	private:

		/*!
		@brief Translates an ASTNode (and the tree below) from libSBML into
				its representation in the engine (an ECellEngine::Maths::Operation).
		@details Works with a heap to go through the ASTNode tree and build our own
				 within a root operation that is returned.
		@param _rootAstNode The root from which we translate the equation.
		@param _rootName The name we want to associate to the operation.
		@param _dataState Reference to a datastate containing named variables that
				may be used in the operation.
		@param _docIdsToDataStateNames Reference to a local unordered map between the
				names used as IDs in SBML and the names we use in the data state.
				They may be identical. This object is instantiated in ::TryImport and
				survives only during import.
		@returns The root ECellEngine::Maths::Operation that represents the equation.
		*/
		Operation ASTNodeToOperation(
			const ASTNode* _rootAstNode, const std::string _rootName,
			ECellEngine::Data::DataState& _dataState,
			std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

		/*!
		@brief Assigns a ECellEngine::Maths::Function to @p _op depending on the
				type of operation of @p _nope.
		@remarks It uses a switch statement with no default. So, in the event that
				 the type of @p _node is not described, the function member of @p _op
				 will stay equal to @a nullptr
		*/
		void AssignOperationFunction(Operation& _op, const ASTNode* _node);

		/*!
		@brief Parses the list of parameters encoded in the sbml file.
		*/
		void InitializeParameters(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

		/*!
		@brief Parses the list of reactions encoded in the sbml file.
		*/
		void InitializeReactions(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

		/*!
		@brief Parses the list of species encoded in the sbml file.
		*/
		void InitializeSpecies(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames);

		/*!
		@brief Tests if the ASTNode has a type that is considered as an operation.
		@returns Returns true for:  ASTNodeType_t::AST_PLUS, ASTNodeType_t::AST_MINUS,
				 ASTNodeType_t::AST_TIMES, ASTNodeType_t::AST_DIVIDE, ASTNodeType_t::AST_FUNCTION_POWER.
				 False, otherwise.
		*/
		bool IsASTNodeOperation(const ASTNode* _node);

		/*!
		@brief Performs a suite of checks on the sbml document to insure its integrity.
		@details Validates and pretty-prints the warnings or errors if any.
		@remark Adapted from: example.cpp (https://sbml.org/software/libsbml/5.18.0/docs/formatted/cpp-api/libsbml-example.html).
		@remark This sample program is distributed under a different license than the rest
				 of libSBML.  This program uses the open-source MIT license, as follows:
				 
				 -----------------------------------------------------------------------
				 
				 Copyright (c) 2013-2018 by the California Institute of Technology
				 (California, USA), the European Bioinformatics Institute (EMBL-EBI, UK)
				 and the University of Heidelberg (Germany), with support from the National
				 Institutes of Health (USA) under grant R01GM070923.  All rights reserved.
				 
				 Permission is hereby granted, free of charge, to any person obtaining a
				 copy of this software and associated documentation files (the "Software"),
				 to deal in the Software without restriction, including without limitation
				 the rights to use, copy, modify, merge, publish, distribute, sublicense,
				 and/or sell copies of the Software, and to permit persons to whom the
				 Software is furnished to do so, subject to the following conditions:
				 
				 The above copyright notice and this permission notice shall be included in
				 all copies or substantial portions of the Software.
				 
				 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
				 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
				 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
				 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
				 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
				 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
				 DEALINGS IN THE SOFTWARE.
				 
				 Neither the name of the California Institute of Technology (Caltech), nor
				 of the European Bioinformatics Institute (EMBL-EBI), nor of the University
				 of Heidelberg, nor the names of any contributors, may be used to endorse
				 or promote products derived from this software without specific prior
				 written permission.

				 -----------------------------------------------------------------------

		@author  Akiya Jouraku
		*/
		const bool ValidateSBML(SBMLDocument* _sbmlDocument);

	public:
		/*!
		@brief Tries to parse a file.
		@param _filePath The system's path to the file.
		@param _dataState Reference to a datastate where additional data will be stored
				and containing other named variables that may be used during the import process.
		@returns If successful, an asset object encapsulated in a shared pointer. @a nullptr, otherwise.
		*/
		virtual const std::shared_ptr<ECellEngine::Data::Module> TryImport(const std::filesystem::path& _filePath, ECellEngine::Data::DataState& _dataState) noexcept override;
	};
}