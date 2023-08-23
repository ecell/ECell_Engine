#include "Core/Simulation.hpp"
#include "Util/BinarySearch.hpp"

std::shared_ptr<ECellEngine::Data::Module> ECellEngine::Core::Simulation::AddModule(const std::string& _filePath)
{
	std::shared_ptr<ECellEngine::Data::Module> module = moduleImporterManager.TryImportModule(std::filesystem::path(_filePath), dataState);
	if (module != nullptr)
	{
		modules.push_back(module);
	}
	return module;
}

std::shared_ptr<Solver> ECellEngine::Core::Simulation::AddSolver(const std::string& _solverClassName)
{
	if (_solverClassName == "GillespieNRMRSolver")
	{
		return solvers.emplace_back(std::make_shared<Stochastic::GillespieNRMRSolver>(dataState, _solverClassName));
	}

	if (_solverClassName == "GeneralizedExplicitRK")
	{
		return solvers.emplace_back(std::make_shared<ODE::GeneralizedExplicitRK>(dataState, _solverClassName));
	}
}

const std::size_t ECellEngine::Core::Simulation::FindModuleIdx(const std::size_t _moduleID)
{
	static Data::Module::CompareID compareID;
	std::vector<std::shared_ptr<Data::Module>>::iterator it = Util::BinarySearch::LowerBound(modules.begin(), modules.end(), _moduleID, compareID);
	if (it != modules.end() && (*it)->id == _moduleID)
	{
		return std::distance(modules.begin(), it);
	}
	return SIZE_MAX;
}

const std::size_t ECellEngine::Core::Simulation::FindSolverIdx(const std::size_t _solverID)
{
	static Solvers::Solver::CompareID compareID;
	std::vector<std::shared_ptr<Solvers::Solver>>::iterator it = Util::BinarySearch::LowerBound(solvers.begin(), solvers.end(), _solverID, compareID);
	if (it != solvers.end() && (*it)->id == _solverID)
	{
		return std::distance(solvers.begin(), it);
	}
	return SIZE_MAX;
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

void ECellEngine::Core::Simulation::Start()
{
	timer.SetStartTime();
	for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = modulesToSolversTable.begin(); it != modulesToSolversTable.end(); it++)
	{
		solvers[(*it).second].get()->Start();
	}
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
		solvers[_solverIdx]->Initialize(modules[_moduleIdx].get());
	}
}

void ECellEngine::Core::Simulation::Update(const float& _deltaTime)
{
	timer.Increment(_deltaTime);
	for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = modulesToSolversTable.begin(); it != modulesToSolversTable.end(); it++)
	{
		solvers[(*it).second].get()->Update(timer);
	}
}