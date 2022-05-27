#include <iostream>
#include <string>
#include <sbml/SBMLTypes.h>

class ASTNodeEx : ASTNode
{
	int numChildren = 0;

	int left;
	int right;
	std::string name;
	ASTNodeType_t type;
	float* f_ptrValue;
	int* i_ptrValue;
	
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
		numChildren = _origin->getNumChildren();
	};

	//accessors
	float* getFPointerValue();

	//ASTNodeEx* getLeftChildEx();
	int getLeftChildEx();

	std::string getNameEx();
	
	int* getIPointerValue();

	int getNumChildrenEx();

	//ASTNodeEx* getRightChildEx();
	int getRightChildEx();

	ASTNodeType_t getTypeEx();
	
	float getValueEx();

	//mutators
	//void setLeftChildEx(ASTNodeEx* _node);
	void setLeftChildEx(int _node);

	void setNameEx(const char* _name);

	void setNumChildrenEx(int _nbChildren);

	void setPointerToValue(float* _ptrValue);

	void setPointerToValue(int* _ptrValue);

	//void setRightChildEx(ASTNodeEx* _node);
	void setRightChildEx(int _node);

	//void setTypeEx(ASTNodeType_t _type);

	void setValueEx(float* _value);
};

class ASTEvaluator
{
	std::vector<ASTNodeEx> formulasNodes;
public:
	
	ASTEvaluator() {};

	//accessors
	ASTNodeEx* getNode(int _i);

	//logic
	float Evaluate(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);

	float Evaluate(ASTNodeEx* _node);

	float EvaluateNode(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);

	float EvaluateNode(ASTNodeEx* _node);

	void ExtractVariables(ASTNodeEx* _node, std::unordered_map<std::string, int>* _namesMap, std::vector<int>* _storage);
	
	int Initializes(const ASTNode* _origin);

	bool isSound(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);

	std::string PrettyPrintFormula(const ASTNode* _node, std::unordered_map<std::string, float*>* _namesMap);
	
	std::string PrettyPrintFormula(ASTNodeEx* _node);

	void PrettyPrintTree(const ASTNode* _node, int _lvl);

	void PrettyPrintTree(ASTNodeEx* _node, int _lvl);

	void ReduceVariables(ASTNodeEx* _node, std::unordered_map<std::string, float*>* _namesMap);

	void ReduceVariables(ASTNodeEx* _node, std::unordered_map<std::string, int*>* _namesMap);

	//ASTNodeEx TranslateToEx(const ASTNode* _node);

 };
