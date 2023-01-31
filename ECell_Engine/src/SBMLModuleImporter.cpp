#include "SBMLModuleImporter.hpp"

void ECellEngine::IO::SBMLModuleImporter::InitializeParameters(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule,
    const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    unsigned int nbParameters = _model->getNumParameters();
    const LIBSBML_CPP_NAMESPACE::Parameter* param;

    unsigned int nbSimpleParameters = 0;
    unsigned int nbComputedParameters = 0;
    for (unsigned int i = 0; i < nbParameters; i++)
    {
        param = _model->getParameter(i);
        if (param->getConstant())
        {
            nbSimpleParameters++;
        }
        else
        {
            nbComputedParameters++;
        }
    }

    _sbmlModule.ResizeSimpleParameters(nbSimpleParameters);
    _sbmlModule.ResizeComputedParameters(nbComputedParameters);

    for (unsigned int i = 0; i < nbParameters; i++)
    {
        param = _model->getParameter(i);
        if (param->getConstant())
        {
            _sbmlModule.AddSimpleParameter(param->getId(), (const float)param->getValue());
            _docIdsToDataStateNames[param->getId()] = param->getId();
        }
    }

    //Processes the parameters defined through rules
    unsigned int nbRules = _model->getNumRules();
    const LIBSBML_CPP_NAMESPACE::Rule* rule;
    const LIBSBML_CPP_NAMESPACE::ASTNode* astNode;
    for (unsigned int i = 0; i < nbRules; i++)
    {
        //std::cout << std::endl;
        rule = _model->getRule(i);
        if (rule->isParameter())
        {
            astNode = rule->getMath();
            Operation root = ASTNodeToOperation(_dataState, rule->getVariable(), astNode, _docIdsToDataStateNames);

            _sbmlModule.AddComputedParameters(rule->getVariable(), root);
            _docIdsToDataStateNames[rule->getVariable()] = rule->getVariable();
        }
    }

    //TODO: make the dependency graph of the parameters
}

void ECellEngine::IO::SBMLModuleImporter::InitializeReactions(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model,
    std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    unsigned int nbReactions = _model->getNumReactions();
    _sbmlModule.ResizeReactions(nbReactions);
    const LIBSBML_CPP_NAMESPACE::Reaction* reaction;

    unsigned int nbReactants;
    unsigned int nbProducts;
    std::vector<std::string> reactants;
    std::vector<std::string> products;
    const LIBSBML_CPP_NAMESPACE::ASTNode* astNode;
    for (unsigned int i = 0; i < nbReactions; i++)
    {
        reaction = _model->getReaction(i);
        nbReactants = reaction->getNumReactants();
        nbProducts = reaction->getNumProducts();

        reactants.resize(nbReactants);
        for (unsigned int j = 0; j < nbReactants; j++)
        {
            reactants.push_back(reaction->getReactant(j)->getId());
        }

        products.resize(nbProducts);
        for (unsigned int j = 0; j < nbProducts; j++)
        {
            products.push_back(reaction->getProduct(j)->getId());
        }

        astNode = reaction->getKineticLaw()->getMath();
        ECellEngine::Logging::Logger::GetSingleton().LogDebug("Processing Kinetic Law of reaction: " + reaction->getId());
        Operation root = ASTNodeToOperation(_dataState, reaction->getId(), astNode, _docIdsToDataStateNames);

        _sbmlModule.AddReaction(reaction->getId(), products, reactants, root);

        reactants.clear();
        products.clear();
    }
}

void ECellEngine::IO::SBMLModuleImporter::InitializeSpecies(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::SBMLModule& _sbmlModule, const Model* _model,
    std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    unsigned int nbSpecies = _model->getNumSpecies();
    _sbmlModule.ResizeSpecies(nbSpecies);
    const LIBSBML_CPP_NAMESPACE::Species* sp;
    for (unsigned int i = 0; i < nbSpecies; ++i)
    {
        sp = _model->getSpecies(i);
        _sbmlModule.AddSpecies(sp->getName(), (float)sp->getInitialAmount());
        _docIdsToDataStateNames[sp->getId()] = sp->getName();
    }
}

Operation ECellEngine::IO::SBMLModuleImporter::ASTNodeToOperation(ECellEngine::Data::DataState& _dataState, const std::string& _rootName, const ASTNode* _node,
    const std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    Operation op(_rootName);
    switch (_node->getType())
    {
    case ASTNodeType_t::AST_PLUS:
        ECellEngine::Logging::Logger::GetSingleton().LogDebug("AST_PLUS with " + std::to_string(_node->getNumChildren()) + " children");
        op.Set(&functions.add);
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getLeftChild(), _docIdsToDataStateNames));
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getRightChild(), _docIdsToDataStateNames));
        break;
    case ASTNodeType_t::AST_MINUS:
        op.Set(&functions.minus);
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getLeftChild(), _docIdsToDataStateNames));
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getRightChild(), _docIdsToDataStateNames));
        break;
    case ASTNodeType_t::AST_TIMES:
        ECellEngine::Logging::Logger::GetSingleton().LogDebug("AST_PLUS with " + std::to_string(_node->getNumChildren()) + " children");
        op.Set(&functions.times);
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getLeftChild(), _docIdsToDataStateNames));
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getRightChild(), _docIdsToDataStateNames));
        break;
    case ASTNodeType_t::AST_DIVIDE:
        op.Set(&functions.divide);
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getLeftChild(), _docIdsToDataStateNames));
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getRightChild(), _docIdsToDataStateNames));
        break;
    case ASTNodeType_t::AST_FUNCTION_POWER:
        op.Set(&functions.power);
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getLeftChild(), _docIdsToDataStateNames));
        op.AddOperand(ASTNodeToOperation(_dataState, _rootName, _node->getRightChild(), _docIdsToDataStateNames));
        break;

    case ASTNodeType_t::AST_NAME:
        op.Set(&functions.identity);
        op.AddOperand(_dataState.GetOperand(_docIdsToDataStateNames.find(_node->getName())->second));
        break;

    case ASTNodeType_t::AST_REAL:
        op.Set(&functions.identity);
        op.AddOperand(Constant((float)_node->getValue()));
        break;

    case ASTNodeType_t::AST_INTEGER:
        op.Set(&functions.identity);
        op.AddOperand(Constant((float)_node->getValue()));
        break;
    }

    return op;
}


/**
 *
 * Adapted from:
 *
 * File: example.cpp (https://sbml.org/software/libsbml/5.18.0/docs/formatted/cpp-api/libsbml-example.html)
 * Function : validateExampleSBML(SBMLDocument* sbmlDoc) -> renamed ValidateSBML(SBMLDocument* sbmlDoc)
 * Description: Validates and pretty-prints the warnings or errors if any.
 * @author  Akiya Jouraku
 *
 * <!--------------------------------------------------------------------------
 * This sample program is distributed under a different license than the rest
 * of libSBML.  This program uses the open-source MIT license, as follows:
 *
 * Copyright (c) 2013-2018 by the California Institute of Technology
 * (California, USA), the European Bioinformatics Institute (EMBL-EBI, UK)
 * and the University of Heidelberg (Germany), with support from the National
 * Institutes of Health (USA) under grant R01GM070923.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Neither the name of the California Institute of Technology (Caltech), nor
 * of the European Bioinformatics Institute (EMBL-EBI), nor of the University
 * of Heidelberg, nor the names of any contributors, may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * -------------------------------------------------------------------------->
 */
const bool ECellEngine::IO::SBMLModuleImporter::ValidateSBML(SBMLDocument* _sbmlDoc)
{
    if (!_sbmlDoc || _sbmlDoc->getModel() == NULL)
    {
        ECellEngine::Logging::Logger::GetSingleton().LogError("The SBML Document given for validation is null.");
        return false;
    }

    std::string consistencyMessages;
    std::string validationMessages;
    bool noProblems = true;
    unsigned int numCheckFailures = 0;
    unsigned int numConsistencyErrors = 0;
    unsigned int numConsistencyWarnings = 0;
    unsigned int numValidationErrors = 0;
    unsigned int numValidationWarnings = 0;
    // LibSBML 3.3 is lenient when generating models from scratch using the
    // API for creating objects.  Once the whole model is done and before it
    // gets written out, it's important to check that the whole model is in
    // fact complete, consistent and valid.
    numCheckFailures = _sbmlDoc->checkInternalConsistency();
    if (numCheckFailures > 0)
    {
        noProblems = false;
        for (unsigned int i = 0; i < numCheckFailures; i++)
        {
            const SBMLError* sbmlErr = _sbmlDoc->getError(i);
            if (sbmlErr->isFatal() || sbmlErr->isError())
            {
                ++numConsistencyErrors;
            }
            else
            {
                ++numConsistencyWarnings;
            }
        }
        std::ostringstream oss;
        _sbmlDoc->printErrors(oss);
        consistencyMessages = oss.str();
    }
    // If the internal checks fail, it makes little sense to attempt
    // further validation, because the model may be too compromised to
    // be properly interpreted.
    if (numConsistencyErrors > 0)
    {
        consistencyMessages += "Further validation aborted.";
    }
    else
    {
        numCheckFailures = _sbmlDoc->checkConsistency();
        if (numCheckFailures > 0)
        {
            noProblems = false;
            for (unsigned int i = 0; i < numCheckFailures; i++)
            {
                const SBMLError* sbmlErr = _sbmlDoc->getError(i);
                if (sbmlErr->isFatal() || sbmlErr->isError())
                {
                    ++numValidationErrors;
                }
                else
                {
                    ++numValidationWarnings;
                }
            }
            std::ostringstream oss;
            _sbmlDoc->printErrors(oss);
            validationMessages = oss.str();
        }
    }
    if (noProblems)
        return true;
    else
    {
        if (numConsistencyErrors > 0)
        {
            std::cout << "ERROR: encountered " << numConsistencyErrors
                << " consistency error" << (numConsistencyErrors == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        if (numConsistencyWarnings > 0)
        {
            std::cout << "Notice: encountered " << numConsistencyWarnings
                << " consistency warning" << (numConsistencyWarnings == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        std::cout << std::endl << consistencyMessages;
        if (numValidationErrors > 0)
        {
            std::cout << "ERROR: encountered " << numValidationErrors
                << " validation error" << (numValidationErrors == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        if (numValidationWarnings > 0)
        {
            std::cout << "Notice: encountered " << numValidationWarnings
                << " validation warning" << (numValidationWarnings == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        std::cout << std::endl << validationMessages;
        return (numConsistencyErrors == 0 && numValidationErrors == 0);
    }
}

const std::shared_ptr<ECellEngine::Data::Module> ECellEngine::IO::SBMLModuleImporter::TryImport(const std::filesystem::path& _filePath, ECellEngine::Data::DataState& _dataState) noexcept
{
	// Checks whether the file is okay
    ECellEngine::Logging::Logger::GetSingleton().LogTrace("Trying to read SBML file: " + _filePath.string());
    SBMLDocument* sbmlDoc = readSBMLFromFile(_filePath.string().c_str());

    std::unordered_map<std::string, std::string> docIdsToDataStateNames;

    // if so, then parse the file, build an SBMLModule and return it as a shared pointer
    if (ValidateSBML(sbmlDoc))
    {
        //std::cout << " The SBML validation process for file at: " << _filePath << " is a SUCCESS." << std::endl;

        std::shared_ptr<ECellEngine::Data::SBMLModule> sbmlModule = std::make_shared<ECellEngine::Data::SBMLModule>(_dataState);
        Model* sbmlModel = sbmlDoc->getModel();

        //Build species
        InitializeSpecies(_dataState, *sbmlModule.get(), sbmlModel, docIdsToDataStateNames);

        //Build parameters ; simple (constants) and computed
        InitializeParameters(_dataState, *sbmlModule.get(), sbmlModel, docIdsToDataStateNames);

        //Build reactions
        InitializeReactions(_dataState, *sbmlModule.get(), sbmlModel, docIdsToDataStateNames);
        
        return sbmlModule;
    }
    // if not return a null_ptr
    else
    {
        //std::cout << "  << std::endl;
        ECellEngine::Logging::Logger::GetSingleton().LogError("The SBML validation process for file at : "
            + _filePath.filename().string() + " has FAILED(see errors above).");
        return nullptr;
    }
}

