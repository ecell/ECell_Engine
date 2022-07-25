#include "ASTNode_parser.hpp"

std::string ASTNodeEx::getNameEx()
{
	return name;
}

int ASTNodeEx::getLeftChildEx()
{
	//return children.first;
	return left;
}

int ASTNodeEx::getNumChildrenEx()
{
	return numChildren;
}

int ASTNodeEx::getRightChildEx()
{
	//return children.second;
	return right;
}

ASTNodeType_t ASTNodeEx::getTypeEx()
{
	return type;
}

float ASTNodeEx::getValueEx()
{
	return value;
}

void ASTNodeEx::setLeftChildEx(int _nodeIdx)
{
	left = _nodeIdx;
}

void ASTNodeEx::setNameEx(const char* _name)
{
	name = _name;
}

//void ASTNodeEx::setNumChildrenEx(int _nbChildren)
//{
//	numChildren = _nbChildren;
//}

void ASTNodeEx::setNumChildrenEx()
{
	if (left > -1)
	{
		numChildren = 1;
		if (right > -1)
		{
			numChildren = 2;
		}
	}
}

void ASTNodeEx::setRightChildEx(int _nodeIdx)
{
	right = _nodeIdx;
}

void ASTNodeEx::setTypeEx(ASTNodeType_t _type)
{
	type = _type;
}

void ASTNodeEx::setValueEx(float _value)
{
	value = _value;
}

ASTNodeEx* ASTEvaluator::getNode(int _i)
{
	return &(formulasNodes)[_i];
}

//float ASTEvaluator::Evaluate(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap)
//{
//	//PrettyPrintTree(_node,0, _namesMap);
//	std::cout << PrettyPrintFormula(_node, _namesMap) << std::endl;
//	return EvaluateNode(_node, _namesMap);
//}
//
//float ASTEvaluator::EvaluateNode(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap)
//{
//	//PrettyPrintTree(_node,0, _namesMap);
//	//std::cout << PrettyPrintFormula(_node, _namesMap) << std::endl;
//	switch (_node->getType())
//	{
//	case ASTNodeType_t::AST_NAME:
//		return *(*_namesMap)[_node->getName()];
//		break;
//	case ASTNodeType_t::AST_REAL:
//		return _node->getValue();
//		break;
//	case ASTNodeType_t::AST_INTEGER:
//		return _node->getValue();
//		break;
//	case ASTNodeType_t::AST_PLUS:
//		return EvaluateNode(_node->getLeftChild(), _namesMap) + EvaluateNode(_node->getRightChild(), _namesMap);
//		break;
//	case ASTNodeType_t::AST_MINUS:
//		return EvaluateNode(_node->getLeftChild(), _namesMap) - EvaluateNode(_node->getRightChild(), _namesMap);
//		break;
//	case ASTNodeType_t::AST_TIMES:
//		return EvaluateNode(_node->getLeftChild(), _namesMap) * EvaluateNode(_node->getRightChild(), _namesMap);
//		break;
//	case ASTNodeType_t::AST_DIVIDE:
//		return EvaluateNode(_node->getLeftChild(), _namesMap) / EvaluateNode(_node->getRightChild(), _namesMap);
//		break;
//	default:
//		return 0;
//	}
//}


//std::string PrintType(const ASTNodeType_t _type)
//{
//
//	switch (_type)
//	{
//	case ASTNodeType_t::AST_NAME:
//		return "AST_NAME";
//		break;
//	case ASTNodeType_t::AST_REAL:
//		return "AST_REAL";
//		break;
//	case ASTNodeType_t::AST_INTEGER:
//		return "AST_INTEGER";
//		break;
//	case ASTNodeType_t::AST_PLUS:
//		return "AST_PLUS";
//		break;
//	case ASTNodeType_t::AST_MINUS:
//		return "AST_MINUS";
//		break;
//	case ASTNodeType_t::AST_TIMES:
//		return "AST_TIMES";
//		break;
//	case ASTNodeType_t::AST_DIVIDE:
//		return "AST_DIVIDE";
//		break;
//	default:
//		return std::string(1, _type);
//		break;
//	}
//}

float ASTEvaluator::Evaluate(ASTNodeEx* _node)
{
	//PrettyPrintTree(_node,0, _namesMap);
	//std::cout << PrettyPrintFormula(_node, _namesMap) << std::endl;
	return EvaluateNode(_node);
}

float ASTEvaluator::EvaluateNode(ASTNodeEx* _node)
{
	//PrettyPrintTree(_node,0, _namesMap);
	//std::cout << PrettyPrintFormula(_node, _namesMap) << std::endl;
	switch (_node->getTypeEx())
	{
	case ASTNodeType_t::AST_NAME:
		/*if (_node->getFPointerValue() != NULL)
		{
			return *_node->getFPointerValue();
		}
		else if (_node->getIPointerValue() != NULL)
		{
			return *_node->getIPointerValue();
		}*/
		return EvaluateNode(getNode(_node->getLeftChildEx()));
		break;
	case ASTNodeType_t::AST_REAL:
		return _node->getValueEx();
		break;
	case ASTNodeType_t::AST_INTEGER:
		return _node->getValueEx();
		break;
	case ASTNodeType_t::AST_PLUS:
		return EvaluateNode(getNode(_node->getLeftChildEx())) + EvaluateNode(getNode(_node->getRightChildEx()));
		break;
	case ASTNodeType_t::AST_MINUS:
		return EvaluateNode(getNode(_node->getLeftChildEx())) - EvaluateNode(getNode(_node->getRightChildEx()));
		break;
	case ASTNodeType_t::AST_TIMES:
		return EvaluateNode(getNode(_node->getLeftChildEx())) * EvaluateNode(getNode(_node->getRightChildEx()));
		break;
	case ASTNodeType_t::AST_DIVIDE:
		return EvaluateNode(getNode(_node->getLeftChildEx())) / EvaluateNode(getNode(_node->getRightChildEx()));
		break;
	default:
		return 0;
	}
}

void ASTEvaluator::ExtractVariables(ASTNodeEx* _node, std::unordered_map<std::string, int>* _namesMap, std::vector<int>* _storage)
{
	switch (_node->getTypeEx())
	{
	case ASTNodeType_t::AST_NAME:
	{
		//std::cout << "Trying to extract variable " << _node->getNameEx() << "...   " << std::endl;
		auto keyPos = (*_namesMap).find(_node->getNameEx());
		if (keyPos != (*_namesMap).end())
		{
			_storage->push_back(keyPos->second);
			ExtractVariables(getNode(_node->getLeftChildEx()), _namesMap, _storage);
		}
		break;
	}
	default:
		if (_node->getNumChildrenEx() > 0)
		{
			ExtractVariables(getNode(_node->getLeftChildEx()), _namesMap, _storage);
		}

		if (_node->getNumChildrenEx() > 1)
		{
			ExtractVariables(getNode(_node->getRightChildEx()), _namesMap, _storage);
		}
		break;
	}
}

float ASTEvaluator::getNamedNodeValue(int _namedNodeIdx)
{
	return formulasNodes[formulasNodes[_namedNodeIdx].getLeftChildEx()].getValueEx();
}

int ASTEvaluator::Initializes(const ASTNode* _origin, const std::unordered_map<std::string, int>* _nodesMap)
{
	//std::cout << "Processing parent node " << _origin->getId() << std::endl;
	//std::cout << "FormulasNos has size: " << astEvaluator->getNbNodes();
	//int idx = formulasNodes.size() - 1;
	//std::cout << idx+1 << " Nodes were registered." << std::endl;
	//formulasNodes.reserve(1);
	int idx;
	std::string name = _origin->getId();	

	if (_origin->getType() == ASTNodeType_t::AST_NAME)
	{
		name = _origin->getName();
	}

	auto nodeIt = _nodesMap->find(name);
	//std::cout << "Node " << name << " of type " << PrintType(_origin->getType()) << " is NOT in _nodesMap: " << (nodeIt == _nodesMap->end()) << std::endl;
	
	if (_origin->getType() != ASTNodeType_t::AST_NAME || nodeIt == _nodesMap->end())
	{
		formulasNodes.push_back(ASTNodeEx(_origin));
		idx = formulasNodes.size() - 1;
	}
	else
	{
		idx = nodeIt->second;
	}

	//std::cout << idx+1 << " Nodes were registered." << std::endl;
	if (_origin->getNumChildren() > 0)
	{
		//std::cout << "Processing left child " << std::endl;
		int left = Initializes(_origin->getLeftChild(), _nodesMap);
		formulasNodes[idx].setLeftChildEx(left);
	}

	if (_origin->getNumChildren() > 1)
	{
		//std::cout << "Processing right child " << std::endl;		
		int right = Initializes(_origin->getRightChild(), _nodesMap);
		formulasNodes[idx].setRightChildEx(right);
	}

	//std::cout << "Returning link to node " << idx << std::endl;
	return idx;
}
//
//bool ASTEvaluator::isSound(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap)
//{
//	//std::cout << "Scanning for parameters; Current node of type " << _node->getTypeEx() << " for reduction." << std::endl;
//	switch (_node->getType())
//	{
//	case ASTNodeType_t::AST_NAME:
//	{
//		auto keyPos = (*_namesMap).find(_node->getName());
//		//std::cout << "Variable " << _node->getName() << " with value " << (*keyPos->second) << " is sound: " << !isnan(*keyPos->second) << std::endl;
//		return !isnan(*keyPos->second);
//		break;
//	}
//	default:
//
//		if (_node->getNumChildren() == 0)
//		{
//			return true;
//		}
//
//		else if (_node->getNumChildren() == 1)
//		{
//			return isSound(_node->getLeftChild(), _namesMap);
//		}
//
//		else if (_node->getNumChildren() == 2)
//		{
//			return isSound(_node->getLeftChild(), _namesMap) && isSound(_node->getRightChild(), _namesMap);
//		}
//
//		break;
//	}
//}

//std::string ASTEvaluator::PrettyPrintFormula(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap)
//{
//	switch (_node->getType())
//	{
//	case ASTNodeType_t::AST_NAME:
//		return std::to_string(*(*_namesMap)[_node->getName()]);
//		break;
//	case ASTNodeType_t::AST_REAL:
//		return std::to_string(_node->getValue());
//		break;
//	case ASTNodeType_t::AST_INTEGER:
//		return std::to_string(_node->getValue());
//		break;
//	case ASTNodeType_t::AST_PLUS:
//		return PrettyPrintFormula(_node->getLeftChild(), _namesMap) + "+" + PrettyPrintFormula(_node->getRightChild(), _namesMap);
//		break;
//	case ASTNodeType_t::AST_MINUS:
//		return PrettyPrintFormula(_node->getLeftChild(), _namesMap) + "-" + PrettyPrintFormula(_node->getRightChild(), _namesMap);
//		break;
//	case ASTNodeType_t::AST_TIMES:
//		return PrettyPrintFormula(_node->getLeftChild(), _namesMap) + "*" + PrettyPrintFormula(_node->getRightChild(), _namesMap);
//		break;
//	case ASTNodeType_t::AST_DIVIDE:
//		return PrettyPrintFormula(_node->getLeftChild(), _namesMap) + "/" + PrettyPrintFormula(_node->getRightChild(), _namesMap);
//		break;
//	default:
//		return "UNKNOWN";
//	}
//}

std::string ASTEvaluator::PrettyPrintFormula(ASTNodeEx* _node)
{
	switch (_node->getTypeEx())
	{
	case ASTNodeType_t::AST_NAME:
		/*if (_node->getFPointerValue() != NULL)
		{
			return std::to_string(*_node->getFPointerValue());
		}
		else if (_node->getIPointerValue() != NULL)
		{
			return std::to_string(*_node->getIPointerValue());
		}*/
		return PrettyPrintFormula(getNode(_node->getLeftChildEx()));
		break;
	case ASTNodeType_t::AST_REAL:
		return std::to_string(_node->getValueEx());
		break;
	case ASTNodeType_t::AST_INTEGER:
		return std::to_string(_node->getValueEx());
		break;
	case ASTNodeType_t::AST_PLUS:
		return PrettyPrintFormula(getNode(_node->getLeftChildEx())) + "+" + PrettyPrintFormula(getNode(_node->getRightChildEx()));
		break;
	case ASTNodeType_t::AST_MINUS:
		return PrettyPrintFormula(getNode(_node->getLeftChildEx())) + "-" + PrettyPrintFormula(getNode(_node->getRightChildEx()));
		break;
	case ASTNodeType_t::AST_TIMES:
		return PrettyPrintFormula(getNode(_node->getLeftChildEx())) + "*" + PrettyPrintFormula(getNode(_node->getRightChildEx()));
		break;
	case ASTNodeType_t::AST_DIVIDE:
		return PrettyPrintFormula(getNode(_node->getLeftChildEx())) + "/" + PrettyPrintFormula(getNode(_node->getRightChildEx()));
		break;
	default:
		return "UNKNOWN";
	}
}


//void ASTEvaluator::PrettyPrintTree(const ASTNode* _node, int _lvl)
//{
//	switch (_node->getType())
//	{
//	case ASTNodeType_t::AST_NAME:
//		std::cout << std::string(_lvl, '  ') << "|-AST_NAME (" << _node->getName() << ", " << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	case ASTNodeType_t::AST_REAL:
//		std::cout << std::string(_lvl, '  ') << "|-AST_REAL (" << _node->getValue() << ", " << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	case ASTNodeType_t::AST_INTEGER:
//		std::cout << std::string(_lvl, '  ') << "|-AST_INTEGER (" << _node->getValue() << ", " << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	case ASTNodeType_t::AST_PLUS:
//		std::cout << std::string(_lvl, '  ') << "|-AST_PLUS (+" << ", " << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	case ASTNodeType_t::AST_MINUS:
//		std::cout << std::string(_lvl, '  ') << "|-AST_MINUS (-" << ", " << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	case ASTNodeType_t::AST_TIMES:
//		std::cout << std::string(_lvl, '  ') << "|-AST_TIMES (x" << ", " << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	case ASTNodeType_t::AST_DIVIDE:
//		std::cout << std::string(_lvl, '  ') << "|-AST_DIVIDE (/" << ", " << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	default:
//		std::cout << std::string(_lvl, '  ') << "Child is of type " << _node->getType() << "(" << _node->getNumChildren() << " children)" << std::endl;
//		break;
//	}
//	for (short i = 0; i < _node->getNumChildren(); i++)
//	{
//		PrettyPrintTree(_node->getChild(i), _lvl + 1);
//	}
//}

void ASTEvaluator::PrettyPrintTree(ASTNodeEx* _node, int _lvl)
{
	switch (_node->getTypeEx())
	{
	case ASTNodeType_t::AST_NAME:
		std::cout << std::string(_lvl, '  ') << "|-AST_NAME (" << _node->getNameEx() << ", " << _node->getNumChildrenEx() << " children)" << std::endl;
		break;
	case ASTNodeType_t::AST_REAL:
		std::cout << std::string(_lvl, '  ') << "|-AST_REAL (" << _node->getValueEx() << ", " << _node->getNumChildrenEx() << " children)" << std::endl;
		break;
	case ASTNodeType_t::AST_INTEGER:
		std::cout << std::string(_lvl, '  ') << "|-AST_INTEGER (" << _node->getValueEx() << ", " << _node->getNumChildrenEx() << " children)" << std::endl;
		break;
	case ASTNodeType_t::AST_PLUS:
		std::cout << std::string(_lvl, '  ') << "|-AST_PLUS (+" << ", " << _node->getNumChildrenEx() << " children)" << std::endl;
		break;
	case ASTNodeType_t::AST_MINUS:
		std::cout << std::string(_lvl, '  ') << "|-AST_MINUS (-" << ", " << _node->getNumChildrenEx() << " children)" << std::endl;
		break;
	case ASTNodeType_t::AST_TIMES:
		std::cout << std::string(_lvl, '  ') << "|-AST_TIMES (x" << ", " << _node->getNumChildrenEx() << " children)" << std::endl;
		break;
	case ASTNodeType_t::AST_DIVIDE:
		std::cout << std::string(_lvl, '  ') << "|-AST_DIVIDE (/" << ", " << _node->getNumChildrenEx() << " children)" << std::endl;
		break;
	default:
		std::cout << std::string(_lvl, '  ') << "Child is of type " << _node->getTypeEx() << std::endl;
		break;
	}

	if (_node->getNumChildrenEx() > 0)
	{
		PrettyPrintTree(getNode(_node->getLeftChildEx()), _lvl + 1);
	}

	if (_node->getNumChildrenEx() > 1)
	{
		PrettyPrintTree(getNode(_node->getRightChildEx()), _lvl + 1);
	}
}

void ASTEvaluator::setNamedNodeValue(int _namedNodeIdx, float _value)
{
	formulasNodes[formulasNodes[_namedNodeIdx].getLeftChildEx()].setValueEx(_value);
}

int ASTEvaluator::getNbNodes()
{
	return formulasNodes.size();
}

//void ASTEvaluator::ReduceVariables(ASTNodeEx* _node, std::unordered_map<std::string, float*>* _namesMap)
//{
//	//std::cout << "Scanning for parameters; Current node of type " << _node->getTypeEx() << " for reduction." << std::endl;
//	switch (_node->getTypeEx())
//	{
//	case ASTNodeType_t::AST_NAME:
//	{
//		//std::cout << "Reducing variable " << _node->getNameEx() << "...   " << std::endl;
//		auto keyPos = (*_namesMap).find(_node->getNameEx());
//		if (keyPos != (*_namesMap).end())
//		{
//			//std::cout << "Key found " << keyPos->first << "; value=" << keyPos->second << std::endl;
//			_node->setPointerToValue(keyPos->second);
//		}
//		break;
//	}
//	default:
//		if (_node->getNumChildrenEx() > 0)
//		{
//			ReduceVariables(&formulasNodes[_node->getLeftChildEx()], _namesMap);
//		}
//
//		if (_node->getNumChildrenEx() > 1)
//		{
//			ReduceVariables(&formulasNodes[_node->getRightChildEx()], _namesMap);
//		}
//		
//		break;
//	}
//}
//
//void ASTEvaluator::ReduceVariables(ASTNodeEx* _node, std::unordered_map<std::string, int*>* _namesMap)
//{
//	//std::cout << "Scanning for quantities; Current node of type " << _node->getTypeEx() << " for reduction." << std::endl;
//	switch (_node->getTypeEx())
//	{
//	case ASTNodeType_t::AST_NAME:
//	{
//		//std::cout << "Trying to reduce variable " << _node->getNameEx() << "...   " << std::endl;
//		auto keyPos = (*_namesMap).find(_node->getNameEx());
//		if (keyPos != (*_namesMap).end())
//		{
//			//std::cout << "Key found " << keyPos->first << "; value=" << keyPos->second << std::endl;
//			_node->setPointerToValue(keyPos->second);
//		}
//		break;
//	}
//	default:
//		if (_node->getNumChildrenEx() > 0)
//		{
//			ReduceVariables(&formulasNodes[_node->getLeftChildEx()], _namesMap);
//		}
//
//		if (_node->getNumChildrenEx() > 1)
//		{
//			ReduceVariables(&formulasNodes[_node->getRightChildEx()], _namesMap);
//		}
//		break;
//	}
//}

