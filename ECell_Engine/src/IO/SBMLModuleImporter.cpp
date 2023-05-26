#include "IO/SBMLModuleImporter.hpp"

void ECellEngine::IO::SBMLModuleImporter::InitializeEquations(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::BiochemicalModule& _sbmlModule,
    const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    //Processes the parameters defined through rules
    unsigned int nbRules = _model->getNumRules();
    const LIBSBML_CPP_NAMESPACE::Rule* rule;
    const LIBSBML_CPP_NAMESPACE::ASTNode* astNode;
    Operation root;
    Operand* lhs;
    std::string variableId;
    std::string variableName;
    for (unsigned int i = 0; i < nbRules; i++)
    {
        //std::cout << std::endl;
        rule = _model->getRule(i);
        astNode = rule->getMath();
        variableId = rule->getVariable();
        variableName = _docIdsToDataStateNames.find(variableId)->second;
        //covers the case when we have a rule of the form 'a = b'
        //In this situation we do not do the tree parsing but we create the
        //operation directly with the root node.
        if (astNode->getType() == ASTNodeType_t::AST_NAME)
        {
            root = Operation(variableId);
            root.Set(&functions.identity);
            root.AddOperand(_dataState.GetOperand(_docIdsToDataStateNames.find(astNode->getName())->second));
        }
        else
        {
            root = ASTNodeToOperation(astNode, variableId, _dataState, _docIdsToDataStateNames);
        }

        if (rule->isParameter())
        {
            lhs = _dataState.GetParameter(variableName).get();
            _sbmlModule.AddEquation(lhs, root);
            _dataState.GetEquation(variableName)->Compute();
            _docIdsToDataStateNames[variableId] = variableName;
        }
        else if (rule->isSpeciesConcentration())
        {
            lhs = _dataState.GetSpecies(variableName).get();
            _sbmlModule.AddEquation(lhs, root);
            _dataState.GetEquation(variableName)->Compute();
            _docIdsToDataStateNames[variableId] = variableName;
		}
        else
        {
            ECellEngine::Logging::Logger::GetSingleton().LogError("Rule " + rule->getVariable() + " is neither a parameter nor a species concentration. This rule was skipped. Please, check that the variable name of this rule matches a parameter or species name.");
        }
    }
}

void ECellEngine::IO::SBMLModuleImporter::InitializeParameters(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::BiochemicalModule& _sbmlModule,
    const Model* _model, std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    unsigned int nbParameters = _model->getNumParameters();
    const LIBSBML_CPP_NAMESPACE::Parameter* param;

    for (unsigned int i = 0; i < nbParameters; i++)
    {
        param = _model->getParameter(i);
        if (param->isSetValue())
        {
			_sbmlModule.AddParameter(param->getId(), (const float)param->getValue());
        }
		else
        {
            _sbmlModule.AddParameter(param->getId(), 0.0f);
        }
        _docIdsToDataStateNames[param->getId()] = param->getId();
    }
}

void ECellEngine::IO::SBMLModuleImporter::InitializeReactions(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::BiochemicalModule& _sbmlModule, const Model* _model,
    std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    unsigned int nbReactions = _model->getNumReactions();
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

        reactants.reserve(nbReactants);
        for (unsigned int j = 0; j < nbReactants; j++)
        {
            reactants.push_back(_docIdsToDataStateNames[reaction->getReactant(j)->getSpecies()]);
        }

        products.reserve(nbProducts);
        for (unsigned int j = 0; j < nbProducts; j++)
        {
            products.push_back(_docIdsToDataStateNames[reaction->getProduct(j)->getSpecies()]);
        }

        astNode = reaction->getKineticLaw()->getMath();
        //covers the case when we have a rule of the form 'a = b'
        //In this situation we do not do the tree parsing but we create the
        //operation directly with the root node.
        if (astNode->getType() == ASTNodeType_t::AST_NAME)
        {
            Operation root = Operation(reaction->getId());
            root.Set(&functions.identity);
            root.AddOperand(_dataState.GetOperand(_docIdsToDataStateNames.find(astNode->getName())->second));
            _sbmlModule.AddReaction(reaction->getId(), products, reactants, root);
        }
        else
        {
            Operation root = ASTNodeToOperation(astNode, reaction->getId(), _dataState, _docIdsToDataStateNames);
            _sbmlModule.AddReaction(reaction->getId(), products, reactants, root);
        }

        reactants.clear();
        products.clear();
    }
}

void ECellEngine::IO::SBMLModuleImporter::InitializeSpecies(ECellEngine::Data::DataState& _dataState, ECellEngine::Data::BiochemicalModule& _sbmlModule, const Model* _model,
    std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    unsigned int nbSpecies = _model->getNumSpecies();
    const LIBSBML_CPP_NAMESPACE::Species* sp;
    for (unsigned int i = 0; i < nbSpecies; ++i)
    {
        sp = _model->getSpecies(i);
        _sbmlModule.AddSpecies(sp->getName(), (float)sp->getInitialAmount());
        _docIdsToDataStateNames[sp->getId()] = sp->getName();
    }
}

Operation ECellEngine::IO::SBMLModuleImporter::ASTNodeToOperation(
                                        const ASTNode* _rootAstNode,const std::string _rootName,
                                        ECellEngine::Data::DataState& _dataState,
                                        std::unordered_map<std::string, std::string>& _docIdsToDataStateNames)
{
    Operation op = Operation(_rootName);
    AssignOperationFunction(op, _rootAstNode);

    std::vector<Operation*> opsStack = { &op };
    std::vector<ASTNode*> nodesStack = { _rootAstNode->getRightChild(), _rootAstNode->getLeftChild()};

    //The while loop to build the tree of operations
    while (opsStack.size() != 0)
    {
        Operation* pOP = opsStack.back(); //pOP stands for parent Operation

        if (pOP->IsFull())
        {
            opsStack.pop_back();
        }
        else
        {
            ASTNode* cNode = nodesStack.back(); //cNode stands for child Node
            if (IsASTNodeOperation(cNode))
            {
                Operation* cOP = &pOP->AddOperation(_rootName);//cOP stands for child Operation
                AssignOperationFunction(*cOP, cNode);
                opsStack.push_back(cOP);

                nodesStack.pop_back();
                nodesStack.push_back(cNode->getRightChild());
                nodesStack.push_back(cNode->getLeftChild());
            }

            if (cNode->getType() == ASTNodeType_t::AST_INTEGER ||
                cNode->getType() == ASTNodeType_t::AST_REAL)
            {
                pOP->AddConstant((float)cNode->getValue());
                nodesStack.pop_back();
            }

            if (cNode->getType() == ASTNodeType_t::AST_NAME)
            {
                //Here the cNode corresponds to a variable (species or parameter)
                //that has already been added to the data state. So we add an Operand
                //directly in the pOP and we find the reference in the data state.
                pOP->AddOperand(_dataState.GetOperand(_docIdsToDataStateNames.find(cNode->getName())->second));
                _dataState.LinkOperandToOperation(_docIdsToDataStateNames.find(cNode->getName())->second, _rootName);
                nodesStack.pop_back();
            }            
        }
        
    }

    // Go through the operation tree and shrink local vectors (constants & operations) before
    // pushing the their content inside the operands vector. Indeed since operands is a
    // vector<Operand*> we cannot push pointers into it until the tree is finished building 
    // and before we shrink. If we did, pointers would be broken every time the contants or
    // operation vectors are changed.
    op.LinkLocalOperands();

    return op;
}

bool ECellEngine::IO::SBMLModuleImporter::IsASTNodeOperation(const ASTNode* _node)
{
    return (_node->getType() == ASTNodeType_t::AST_PLUS ||
            _node->getType() == ASTNodeType_t::AST_MINUS ||
            _node->getType() == ASTNodeType_t::AST_TIMES ||
            _node->getType() == ASTNodeType_t::AST_DIVIDE ||
            _node->getType() == ASTNodeType_t::AST_FUNCTION_POWER ||
            _node->getType() == ASTNodeType_t::AST_FUNCTION_ROOT);
}

void ECellEngine::IO::SBMLModuleImporter::AssignOperationFunction(Operation& _op, const ASTNode* _node)
{
    switch (_node->getType())
    {
    case ASTNodeType_t::AST_PLUS:
        _op.Set(&functions.add);
        break;

    case ASTNodeType_t::AST_MINUS:
        _op.Set(&functions.minus);
        break;

    case ASTNodeType_t::AST_TIMES:
        _op.Set(&functions.times);
        break;

    case ASTNodeType_t::AST_DIVIDE:
        _op.Set(&functions.divide);
        break;

    case ASTNodeType_t::AST_FUNCTION_POWER:
        _op.Set(&functions.power);
        break;

    case ASTNodeType_t::AST_FUNCTION_ROOT:
        _op.Set(&functions.root);
		break;
    }
}

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

        std::shared_ptr<ECellEngine::Data::BiochemicalModule> sbmlModule = std::make_shared<ECellEngine::Data::BiochemicalModule>(_dataState);
        Model* sbmlModel = sbmlDoc->getModel();

        //Build species
        ECellEngine::Logging::Logger::GetSingleton().LogTrace("Building species...");
        InitializeSpecies(_dataState, *sbmlModule.get(), sbmlModel, docIdsToDataStateNames);

        //Build parameters
        ECellEngine::Logging::Logger::GetSingleton().LogTrace("Building parameters...");
        InitializeParameters(_dataState, *sbmlModule.get(), sbmlModel, docIdsToDataStateNames);

        //Build equations
        ECellEngine::Logging::Logger::GetSingleton().LogTrace("Building equations...");
        InitializeEquations(_dataState, *sbmlModule.get(), sbmlModel, docIdsToDataStateNames);

        //Build reactions
        ECellEngine::Logging::Logger::GetSingleton().LogTrace("Building reactions...");
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

