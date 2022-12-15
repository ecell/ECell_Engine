#include "Simulation.hpp"

void ECellEngine::Core::Simulation::AddModule(const std::string& _filePath)
{
	std::shared_ptr<Module> module = moduleImporterManager.TryImportModule(std::filesystem::path(_filePath), &dataState);
	if (module != nullptr)
	{
		modules.push_back(module);
	}
}

void ECellEngine::Core::Simulation::AddSolver(const std::string& _solverClassName)
{
	if (_solverClassName == "GillespieNRMRSolver")
	{
		std::shared_ptr<Solver> solver = std::make_shared<GillespieNRMRSolver>(&dataState);
		solvers.push_back(solver);
	}
}

const std::pair<std::pair<bool, std::size_t>, std::pair<bool, std::size_t>>
	ECellEngine::Core::Simulation::ScanModuleToSolverTable(const std::size_t& _moduleIdx, const std::size_t& _solverIdx)
{
	std::pair<bool, std::size_t> moduleScan = std::pair(false, 0);
	std::pair<bool, std::size_t> solverScan = std::pair(false, 0);
	for (std::size_t i = 0; i < modulesToSolversTable.size(); i++)
	{
		if (modulesToSolversTable[i].first == _moduleIdx)
		{
			moduleScan.first = true;
			moduleScan.second = i;
		}

		if (modulesToSolversTable[i].second == _solverIdx)
		{
			solverScan.first = true;
			solverScan.second = i;
		}
	}

	return std::pair(moduleScan, solverScan);
}

void ECellEngine::Core::Simulation::RemoveModule(const std::size_t& _idx)
{
	//delete the module
	//remove the key/value pair of the ModuleToSolverMap if the module was used there
}

void ECellEngine::Core::Simulation::RemoveSolver(const std::size_t& _idx)
{
	//delete the solver
	//remove the key/value pair of the ModuleToSolverMap if the solver was used there
}

void ECellEngine::Core::Simulation::TryAttachSolverToModule(const std::size_t& _solverIdx, const std::size_t& _moduleIdx)
{
	//If the type of the solver is adapted for the module
	if (modules[_moduleIdx].get()->IsValidSolverType(solvers[_solverIdx].get()))
	{
		auto search = ScanModuleToSolverTable(_moduleIdx, _solverIdx);

		//If the module already has a solver attached to it
		if (search.first.first)
		{
			//We remove the pair
			modulesToSolversTable.erase(modulesToSolversTable.begin() + search.first.second);
		}

		//If the solver is already attached to another module
		if (search.second.first)
		{
			//We remove the pair
			modulesToSolversTable.erase(modulesToSolversTable.begin() + search.second.second);
		}

		//We create the pair in the table to have the binding information
		modulesToSolversTable.push_back(std::pair(_moduleIdx, _solverIdx));
		//We initialize the solver according to the data stored in the module.
		solvers[_solverIdx].get()->Initialize(*modules[_moduleIdx].get());
	}
}

void ECellEngine::Core::Simulation::Update(const float& _deltaTime)
{

}