#include <iostream>
#include <string>
#include <sbml/SBMLTypes.h>

class ASTNodeEx : ASTNode
{
	int numChildren = 0;

	int left = -1;
	int right = -1;
	float value = -1;
	std::string name = "name";
	ASTNodeType_t type = ASTNodeType_t::AST_UNKNOWN;
	//float* f_ptrValue;
	//int* i_ptrValue;
	
public:

	ASTNodeEx(){};

	ASTNodeEx(const ASTNode* _origin)
	{
		type = _origin->getType();
		//std::cout << "Creating a new node of type " << type << std::endl;
		if (_origin->getType() == ASTNodeType_t::AST_NAME)
		{
			name = _origin->getName();
		}

		if (_origin->isNumber())
		{
			value = _origin->getValue();
		}

		numChildren = _origin->getNumChildren();
	};

	//accessors

	int getLeftChildEx();

	std::string getNameEx();

	int getNumChildrenEx();
	int getRightChildEx();

	ASTNodeType_t getTypeEx();
	
	float getValueEx();

	//mutators

	void setLeftChildEx(int _node);

	void setNameEx(const char* _name);

	void setNumChildrenEx();

	void setRightChildEx(int _node);

	void setTypeEx(ASTNodeType_t _type);

	void setValueEx(float _value);
};

class ASTEvaluator
{
private:
	std::vector<ASTNodeEx> formulasNodes;

public:
	ASTEvaluator(){}

	ASTEvaluator(const ASTEvaluator& astEvaluator) : formulasNodes(astEvaluator.formulasNodes) {};

	//accessors
	ASTNodeEx* getNode(int _i);

	int getNbNodes();

	//logic
	//float Evaluate(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);

	float Evaluate(ASTNodeEx* _node);

	//float EvaluateNode(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);

	float EvaluateNode(ASTNodeEx* _node);

	void ExtractVariables(ASTNodeEx* _node, std::unordered_map<std::string, int>* _namesMap, std::vector<int>* _storage);
	
	float getNamedNodeValue(int _namedNodeIdx);

	int Initializes(const ASTNode* _origin, const std::unordered_map<std::string, int>* _nodesMap);

	//bool isSound(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);

	//std::string PrettyPrintFormula(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);
	
	std::string PrettyPrintFormula(ASTNodeEx* _node);

	//void PrettyPrintTree(const ASTNode* _node, int _lvl);

	void PrettyPrintTree(ASTNodeEx* _node, int _lvl);

	void setNamedNodeValue(int _namedNodeIdx, float _value);
	//void ReduceVariables(ASTNodeEx* _node, std::unordered_map<std::string, float*>* _namesMap);

	//void ReduceVariables(ASTNodeEx* _node, std::unordered_map<std::string, int*>* _namesMap);

	//ASTNodeEx TranslateToEx(const ASTNode* _node);

 };
