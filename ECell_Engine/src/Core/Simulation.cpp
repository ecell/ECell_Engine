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

const std::vector<std::shared_ptr<ECellEngine::Data::Module>>::iterator ECellEngine::Core::Simulation::FindModule(const std::size_t _moduleID)
{
	static Data::Module::CompareID compareID;
	return Util::BinarySearch::LowerBound(modules.begin(), modules.end(), _moduleID, compareID);
}

const std::vector<std::pair<std::size_t, std::size_t>>::iterator ECellEngine::Core::Simulation::FindModuleSolverLinkLB(const std::size_t _moduleIdx, const std::size_t _solverIdx)
{
	static CompareLinksFirst compareLinksFirst;
	static CompareLinksSecond compareLinksSecond;

	std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = Util::BinarySearch::LowerBound(moduleSolverLinks.begin(), moduleSolverLinks.end(), std::pair(_moduleIdx, _solverIdx), compareLinksFirst);

	//if the module was not found, return the lower bound directly (end())
	if (lowerBound == moduleSolverLinks.end())
	{
		return lowerBound;
	}

	std::vector<std::pair<std::size_t, std::size_t>>::iterator upperBound = Util::BinarySearch::UpperBound(lowerBound, moduleSolverLinks.end(), std::pair(_moduleIdx, _solverIdx), compareLinksFirst);

	return Util::BinarySearch::LowerBound(lowerBound, upperBound, std::pair(_moduleIdx, _solverIdx), compareLinksSecond);
}

const std::vector<std::shared_ptr<ECellEngine::Solvers::Solver>>::iterator ECellEngine::Core::Simulation::FindSolver(const std::size_t _solverID)
{
	static Solvers::Solver::CompareID compareID;
	return Util::BinarySearch::LowerBound(solvers.begin(), solvers.end(), _solverID, compareID);
}

void ECellEngine::Core::Simulation::RemoveModule(const std::size_t _id)
{
	//Find the module in the ::modules list
	std::vector<std::shared_ptr<Data::Module>>::iterator moduleIt = FindModule(_id);
	std::pair<std::size_t, std::size_t> value(std::distance(modules.begin(), moduleIt), 0);

	//delete the module from the ::modules list
	modules.erase(moduleIt);

	//Find the Range of the module in the ::moduleSolverLinks list
	static CompareLinksFirst compareLinksFirst;
	std::vector<std::pair<std::size_t, std::size_t>>::iterator lowerBound = Util::BinarySearch::LowerBound(moduleSolverLinks.begin(), moduleSolverLinks.end(), value, compareLinksFirst);
	std::vector<std::pair<std::size_t, std::size_t>>::iterator upperBound = Util::BinarySearch::UpperBound(lowerBound, moduleSolverLinks.end(), value, compareLinksFirst);

	//delete the module from the ::moduleSolverLinks list
	moduleSolverLinks.erase(lowerBound, upperBound);
}

void ECellEngine::Core::Simulation::RemoveSolver(const std::size_t _id)
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

void ECellEngine::Core::Simulation::TryModuleSolverLink(const std::size_t& _solverID, const std::size_t& _moduleID)
{
	//search for the module in the ::modules list
	std::vector<std::shared_ptr<Data::Module>>::iterator moduleIt = FindModule(_moduleID);
	//TODO: return an error code if the module is not found

	//search for the solver in the ::solvers list
	std::vector<std::shared_ptr<Solvers::Solver>>::iterator solverIt = FindSolver(_solverID);
	//TODO:return an error code if the solver is not found
	
	//If the type of the solver is adapted for the module
	if ((*moduleIt)->IsValidSolverType(solverIt->get()))
	{
		//We search for the lower bound where to insert the pair <moduleIdx, solverIdx> in ::moduleSolverLinks
		std::size_t moduleIdx = std::distance(modules.begin(), moduleIt);
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