#include "BiochemicalModule.hpp"

void ECellEngine::Solvers::BiochemicalSolver::Initialize(const ECellEngine::Data::Module* _module)
{
	module = dynamic_cast<const ECellEngine::Data::BiochemicalModule*>(_module);
}