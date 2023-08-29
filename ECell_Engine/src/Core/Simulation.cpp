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
	// -- Remove the module from the ::modules list --
	//Find the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Data::Module>>::iterator> moduleSearch = FindModule(_id);
	if (!moduleSearch.first)
	{
		return false;
	}
	
	//Make a pair with the module index (and a dummy value for the solver index)
	std::pair<std::size_t, std::size_t> value(std::distance(modules.begin(), moduleSearch.second), 0);

	//Erase the module from the ::modules list
	modules.erase(moduleSearch.second);

	// -- Remove the pairs with the module index from the ::moduleSolverLinks list --
	//Find the Range of the module in the ::moduleSolverLinks list
	static CompareLinksFirst compareLinksFirst;
	std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = Util::BinarySearch::LowerBound(moduleSolverLinks.begin(), moduleSolverLinks.end(), value, compareLinksFirst);
	
	//if at least one pair was found with the module index
	if (lowerBound->first == value.first)
	{
		//Search for the upper bound of the links that have the same module index
		std::vector<std::pair<std::size_t, std::size_t>>::iterator upperBound = Util::BinarySearch::UpperBound(lowerBound, moduleSolverLinks.end(), value, compareLinksFirst);
		
		//Erase the all pairs with the moduleIdx from the ::moduleSolverLinks list
		moduleSolverLinks.erase(lowerBound, upperBound);

		//decrement the module index of the links that have a module index greater than the deleted module index
		for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = upperBound; it != moduleSolverLinks.end(); ++it)
		{
			(*it).first--;
		}
	}
	
	return true;
}

bool ECellEngine::Core::Simulation::RemoveModuleSolverLink(const std::size_t _moduleID, const std::size_t _solverID)
{
	//Search for the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<Data::Module>>::iterator> moduleSearch = FindModule(_moduleID);
	if (!moduleSearch.first)
	{
		return false;
	}

	//Search for the solver in the ::solvers list
	std::pair<bool, std::vector<std::shared_ptr<Solvers::Solver>>::iterator> solverSearch = FindSolver(_solverID);
	if (!solverSearch.first)
	{
		return false;
	}

	//Search for the link in the ::moduleSolverLinks list
	std::size_t moduleIdx = std::distance(modules.begin(), moduleSearch.second);
	std::size_t solverIdx = std::distance(solvers.begin(), solverSearch.second);
	std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> linkSearch = FindModuleSolverLink(moduleIdx, solverIdx);
	if (!linkSearch.first)
	{
		return false;
	}

	//Erase the link from the ::moduleSolverLinks list
	moduleSolverLinks.erase(linkSearch.second);
	return true;
}

bool ECellEngine::Core::Simulation::RemoveSolver(const std::size_t _id)
{
	// -- Remove the solver from the ::solvers list --
	//Find the solver in the ::solvers list
	std::pair<bool, std::vector<std::shared_ptr<ECellEngine::Solvers::Solver>>::iterator> solverSearch = FindSolver(_id);
	if (!solverSearch.first)
	{
		return false;
	}

	//Make a pair with the solver index (and a dummy value for the module index)
	std::pair<std::size_t, std::size_t> value(0, std::distance(solvers.begin(), solverSearch.second));

	//Erase the solver from the ::solvers list
	solvers.erase(solverSearch.second);

	// -- Remove the pairs with the solver index from the ::moduleSolverLinks list --
	static CompareLinksFirst compareLinksFirst;
	static CompareLinksSecond compareLinksSecond;
	std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = moduleSolverLinks.begin();

	//Iterate on all ranges starting with an identical module index and delete the links that have the target solver index
	for (std::size_t currentModuleidx = 0; currentModuleidx < modules.size(); ++currentModuleidx)
	{
		//Find the range of the links that have the same module index (and therefore where the solver index might be)
		value.first = currentModuleidx;
		std::vector<std::pair<std::size_t, std::size_t>>::iterator upperBound = Util::BinarySearch::UpperBound(lowerBound, moduleSolverLinks.end(), value, compareLinksFirst);

		//Find the lower bound of the links relatively to the solver index in the range found above
		std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = Util::BinarySearch::LowerBound(lowerBound, upperBound, value, compareLinksSecond);

		//If the lower bound has the same solver index as the one we are looking for, delete it
		if (lowerBound->second == value.second)
		{
			lowerBound = moduleSolverLinks.erase(lowerBound);
		}

		//In any case, decrement the solver index of the links which have a solver index greater than the target solver index
		for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = lowerBound; it != upperBound; ++it)
		{
			(*it).second--;
		}
	}

	return true;
}

void ECellEngine::Core::Simulation::Start()
{
	timer.SetStartTime();
	for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = moduleSolverLinks.begin(); it != moduleSolverLinks.end(); it++)
	{
		solvers[(*it).second].get()->Start();
	}
}

bool ECellEngine::Core::Simulation::TryModuleSolverLink(const std::size_t& _moduleID, const std::size_t& _solverID)
{
	//search for the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<Data::Module>>::iterator> moduleSearch = FindModule(_moduleID);
	if (!moduleSearch.first)
	{
		return false;
	}

	//search for the solver in the ::solvers list
	std::pair<bool, std::vector<std::shared_ptr<Solvers::Solver>>::iterator> solverSearch = FindSolver(_solverID);
	if (!solverSearch.first)
	{
		return false;
	}
	
	//If the type of the solver is adapted for the module
	if ((*(moduleSearch.second))->IsValidSolverType(solverSearch.second->get()))
	{
		//We search for the lower bound where to insert the pair <moduleIdx, solverIdx> in ::moduleSolverLinks
		std::size_t moduleIdx = std::distance(modules.begin(), moduleSearch.second);
		std::size_t solverIdx = std::distance(solvers.begin(), solverSearch.second);

		//We search for the lower bound where to insert the pair <moduleIdx, solverIdx> in ::moduleSolverLinks
		//We do so using the FindModuleSolverLink function which will tell us whether the pair is already in the list or not
		std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> linkSearch = FindModuleSolverLink(moduleIdx, solverIdx);

		//If the pair is already in the list, we return false
		if (linkSearch.first)
		{
			return false;
		}

		moduleSolverLinks.insert(linkSearch.second, std::pair(moduleIdx, solverIdx));
		
		//We initialize the solver with the module
		solvers[solverIdx]->Initialize(modules[moduleIdx].get());

		return true;
	}
	return false;
}

void ECellEngine::Core::Simulation::Update(const float& _deltaTime)
{
	timer.Increment(_deltaTime);
	for (std::vector<std::pair<std::size_t, std::size_t>>::iterator it = moduleSolverLinks.begin(); it != moduleSolverLinks.end(); it++)
	{
		solvers[(*it).second].get()->Update(timer);
	}
}