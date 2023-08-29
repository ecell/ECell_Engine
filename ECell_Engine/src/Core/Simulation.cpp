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

	return nullptr;
}

std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Data::Module>>::iterator> ECellEngine::Core::Simulation::FindModule(const std::size_t _moduleID)
{
	static Data::Module::CompareID compareID;
	std::vector<std::shared_ptr<ECellEngine::Data::Module>>::iterator it = Util::BinarySearch::LowerBound(modules.begin(), modules.end(), _moduleID, compareID);
	if (it != modules.end() && (*it)->id == _moduleID)
	{
		return std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Data::Module>>::iterator>(true, it);
	}
	return std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Data::Module>>::iterator>(false, it);
}

std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> ECellEngine::Core::Simulation::FindModuleSolverLink(const std::size_t _moduleIdx, const std::size_t _solverIdx)
{
	static CompareLinksFirst compareLinksFirst;
	static CompareLinksSecond compareLinksSecond;

	std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = Util::BinarySearch::LowerBound(moduleSolverLinks.begin(), moduleSolverLinks.end(), std::pair(_moduleIdx, _solverIdx), compareLinksFirst);

	//if the module was not found, return the lower bound directly (end())
	if (lowerBound == moduleSolverLinks.end())
	{
		return std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator>(false, lowerBound);
	}

	//if a lower bound was found, but the module is not the one we are looking for, return the lower bound directly
	if ((*lowerBound).first != _moduleIdx)
	{
		return std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator>(false, lowerBound);
	}

	std::vector<std::pair<std::size_t, std::size_t>>::iterator upperBound = Util::BinarySearch::UpperBound(lowerBound, moduleSolverLinks.end(), std::pair(_moduleIdx, _solverIdx), compareLinksFirst);

	lowerBound = Util::BinarySearch::LowerBound(lowerBound, upperBound, std::pair(_moduleIdx, _solverIdx), compareLinksSecond);

	//if the solver was not found, return the lower bound directly (upperBound, it may be end())
	if (lowerBound == upperBound)
	{
		return std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator>(false, lowerBound);
	}

	//if a lower bound was found, but the solver is not the one we are looking for, return the lower bound directly
	if ((*lowerBound).second != _solverIdx)
	{
		return std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator>(false, lowerBound);
	}

	return std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator>(true, lowerBound);
}

std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Solvers::Solver>>::iterator> ECellEngine::Core::Simulation::FindSolver(const std::size_t _solverID)
{
	static Solvers::Solver::CompareID compareID;
	std::vector<std::shared_ptr<ECellEngine::Solvers::Solver>>::iterator it = Util::BinarySearch::LowerBound(solvers.begin(), solvers.end(), _solverID, compareID);
	if (it != solvers.end() && (*it)->id == _solverID)
	{
		return std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Solvers::Solver>>::iterator>(true, it);
	}
	return std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Solvers::Solver>>::iterator>(false, it);
}

bool ECellEngine::Core::Simulation::RemoveModule(const std::size_t _id)
{
	//Find the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Data::Module>>::iterator> moduleSearch = FindModule(_id);
	if (!moduleSearch.first)
	{
		return false;
	}
	std::pair<std::size_t, std::size_t> value(std::distance(modules.begin(), moduleSearch.second), 0);

	//delete the module from the ::modules list
	modules.erase(moduleSearch.second);

	//Find the Range of the module in the ::moduleSolverLinks list
	static CompareLinksFirst compareLinksFirst;
	std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = Util::BinarySearch::LowerBound(moduleSolverLinks.begin(), moduleSolverLinks.end(), value, compareLinksFirst);
	if (*lowerBound != value)
	{
		return false;
	}
	
	std::vector<std::pair<std::size_t, std::size_t>>::iterator upperBound = Util::BinarySearch::UpperBound(lowerBound, moduleSolverLinks.end(), value, compareLinksFirst);

	//delete the module from the ::moduleSolverLinks list
	moduleSolverLinks.erase(lowerBound, upperBound);
}

bool ECellEngine::Core::Simulation::RemoveModuleSolverLink(const std::size_t _moduleID, const std::size_t _solverID)
{
	//search for the module in the ::modules list
	std::vector<std::shared_ptr<Data::Module>>::iterator moduleSearch = FindModule(_moduleID);
	std::size_t moduleIdx = std::distance(modules.begin(), moduleSearch);
	//TODO: return an error code if the module is not found

	//search for the solver in the ::solvers list
	std::vector<std::shared_ptr<Solvers::Solver>>::iterator solverIt = FindSolver(_solverID);
	std::size_t solverIdx = std::distance(solvers.begin(), solverIt);
	//TODO:return an error code if the solver is not found

	std::vector<std::pair<std::size_t, std::size_t>>::iterator moduleSolverLinkIt = FindModuleSolverLinkLB(moduleIdx, solverIdx);

	if (moduleSolverLinkIt != moduleSolverLinks.end() && moduleSolverLinkIt->first == moduleIdx && moduleSolverLinkIt->second == solverIdx)
	{
		moduleSolverLinks.erase(moduleSolverLinkIt);
		return true;
	}

	//TODO: return an error code if the link is not found

	return false;
}

bool ECellEngine::Core::Simulation::RemoveSolver(const std::size_t _id)
{
	//Find the solver in the ::solvers list
	std::vector<std::shared_ptr<Solvers::Solver>>::iterator solverIt = FindSolver(_id);
	std::pair<std::size_t, std::size_t> value(0, std::distance(solvers.begin(), solverIt));

	solvers.erase(solverIt);

	//Iterate with binary search to find and delete all the links using this solver until the end of the ::moduleSolverLinks list
	static CompareLinksSecond compareLinksSecond;
	std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = Util::BinarySearch::LowerBound(moduleSolverLinks.begin(), moduleSolverLinks.end(), value, compareLinksSecond);
	while (lowerBound != moduleSolverLinks.end())
	{
		lowerBound = Util::BinarySearch::LowerBound(moduleSolverLinks.erase(lowerBound), moduleSolverLinks.end(), value, compareLinksSecond);
	}
}

void ECellEngine::Core::Simulation::Start()
{
	timer.SetStartTime();
	for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = moduleSolverLinks.begin(); it != moduleSolverLinks.end(); it++)
	{
		solvers[(*it).second].get()->Start();
	}
}

void ECellEngine::Core::Simulation::TryModuleSolverLink(const std::size_t& _moduleID, const std::size_t& _solverID)
{
	//search for the module in the ::modules list
	std::vector<std::shared_ptr<Data::Module>>::iterator moduleSearch = FindModule(_moduleID);
	//TODO: return an error code if the module is not found

	//search for the solver in the ::solvers list
	std::vector<std::shared_ptr<Solvers::Solver>>::iterator solverIt = FindSolver(_solverID);
	//TODO:return an error code if the solver is not found
	
	//If the type of the solver is adapted for the module
	if ((*moduleSearch)->IsValidSolverType(solverIt->get()))
	{
		//We search for the lower bound where to insert the pair <moduleIdx, solverIdx> in ::moduleSolverLinks
		std::size_t moduleIdx = std::distance(modules.begin(), moduleSearch);
		std::size_t solverIdx = std::distance(solvers.begin(), solverIt);
		std::vector<std::pair<std::size_t, std::size_t>>::iterator it = FindModuleSolverLinkLB(moduleIdx, solverIdx);

		moduleSolverLinks.insert(it, std::pair(moduleIdx, solverIdx));
				
		//TODO: Return an error code if the pair <moduleIdx, solverIdx> is already in ::moduleSolverLinks
		// 
		// We should also probably return errors in the following cases:
		//	- TODO: Return an error code if the module is already linked to another solver
		//	- TODO: Return an error code if the solver is already linked to another module
		// This will change with introduction of the asset system because it will make sense to allow
		// multiple reactions or modules to connect to a biochemical solver (for example).

		//We initialize the solver according to the data stored in the module.
		solvers[solverIdx]->Initialize(modules[moduleIdx].get());
	}
}

void ECellEngine::Core::Simulation::Update(const float& _deltaTime)
{
	timer.Increment(_deltaTime);
	for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = moduleSolverLinks.begin(); it != moduleSolverLinks.end(); it++)
	{
		solvers[(*it).second].get()->Update(timer);
	}
}