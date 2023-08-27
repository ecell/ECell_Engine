#pragma once

#include <filesystem> //for std::filesystem::path
#include <memory> //for std::shared_ptr
#include <vector> //for std::vector<>

#include "Data/DependenciesDatabase.hpp"
#include "IO/ModuleImporterManager.hpp"
#include "Solver/Stochastic/GillespieNRMRSolver.hpp"
#include "Solver/ODE/GeneralizedExplicitRK.hpp"

using namespace ECellEngine::IO;
using namespace ECellEngine::Solvers;

namespace ECellEngine::Core
{
	/*!
	@brief The objects to everything related to the simulation of a model.
	@detilas Contains the assets and solvers. Root requester for asset import.
			 Controls and checks the link between assets and solvers.Controls
			 the update of solvers.
	*/
	class Simulation
	{
	private:
		/*!
		@brief The data state instance of this simulation.
		*/
		ECellEngine::Data::DataState dataState;

		/*!
		@brief The dependencies between the datastructures loaded in this simulation.
		*/
		ECellEngine::Data::DependenciesDatabase dependenciesDB;

		/*!
		@brief The manager to handle data import for this simulation.
		*/
		ModuleImporterManager moduleImporterManager;

		/*!
		@brief The list of imported assets.
		@warning For the search API ::FindModuleIdx(const char* _moduleID) to
				 work accordingly, the number of module in this list must be less
				 or equal to SIZE_MAX-1 (the highest value of std::size_t). This
				 is: modules.size <= SIZE_MAX-1
		*/
		std::vector<std::shared_ptr<ECellEngine::Data::Module>> modules;
		
		/*!
		@brief The list of added solvers in the simulation space.
		@remarks A solver added to a simulation but not attached to any solver
				 will have no impact on the simulation.
		@warning For the search API ::FindSolverIdx(const char* _solverID) to
				 work accordingly, the number of solver in this list must be less
				 or equal to SIZE_MAX-1 (the highest value of std::size_t). This
				 is: solvers.size <= SIZE_MAX-1
		*/
		std::vector<std::shared_ptr<Solver>> solvers;

		/*!
		@brief The list of module-solver links (the module is used as input by
				the solver).
		@details This is list is sorted by the module index first and then by the
				 solver index.
				 First item is the index to find the module in the vector ::modules.
				 Second item is the index to find the module in the vector ::solvers.
				 It is possible to do a stable insertion of any new link by using
				 ::FindModuleSolverLinkIdx(const std::size_t _moduleIdx, const std::size_t _solverIdx)
				 to find the pointer to the position where to insert the new link.
				 
		*/
		std::vector<std::pair<std::size_t, std::size_t>> moduleSolverLinks;

		/*!
		@brief The simulation's timing information
		@details Namely, contains information about the deltaTime of the simulation.
		*/
		Timer timer;

	public:

		/*!
		@brief The comparison function object to compare the first item of a pair
				in ::moduleSolverLinks.
		*/
		struct CompareLinksFirst
		{
			/*!
			@brief Compares if @p _lhs.first is less than @p _rhs.first.
			@param _lhs The left hand side of the comparison.
			@param _rhs The right hand side of the comparison.
			@return True if @p _lhs.first < @p _rhs.first; false otherwise.
			*/
			bool operator()(const std::pair<std::size_t, std::size_t>& _lhs, const std::pair<std::size_t, std::size_t>& _rhs) const
			{
				return _lhs.first < _rhs.first;
			}
		};

		/*!
		@brief The comparison function object to compare the second item of a pair
				in ::moduleSolverLinks.
		*/
		struct CompareLinksSecond
		{
			/*!
			@brief Compares if @p _lhs.second is less than @p _rhs.second.
			@param _lhs The left hand side of the comparison.
			@param _rhs The right hand side of the comparison.
			@return True if @p _lhs.second < @p _rhs.second; false otherwise.
			*/
			bool operator()(const std::pair<std::size_t, std::size_t>& _lhs, const std::pair<std::size_t, std::size_t>& _rhs) const
			{
				return _lhs.second < _rhs.second;
			}
		};

		/*!
		@brief The unique identifier of this simulation.
		*/
		const std::size_t id;

		Simulation(const std::size_t _id) :
			id(_id)
		{

		}

		/*!
		@brief Tries to add the file at path @p _filePath as an asset to the simulation.
		@param _filePath The directory where to look for the target file. This is the 
						 complete directory with the name the file at the end.
		@return The shared pointer encapsulating the module if the file was successfully
				loaded as an asset. Otherwise, returns nullptr.
		@remaks This method is accessible from outside via the command
				ECellEngine::IO::AddModuleCommand.
		*/
		std::shared_ptr<ECellEngine::Data::Module> AddModule(const std::string& _filePath);

		/*!
		@brief Adds a solver of type indicated by @p _solverClassName.
		@param _solverClassName The name of the type of solver to add. It must
								match a name of class deriving from
								ECellEngine::Solvers::Solver.
		@return The shared pointer encapsulating the solver.
		*/
		std::shared_ptr<Solver> AddSolver(const std::string& _solverClassName);

		inline ECellEngine::Data::DataState* GetDataState()
		{
			return &dataState;
		}

		/*!
		@brief Gets the dependencies of the datastructure loaded in this simulation.
		@returns ::dependenciesDB
		*/
		inline const ECellEngine::Data::DependenciesDatabase& GetDependenciesDatabase() const noexcept
		{
			return dependenciesDB;
		}

		/*!
		@brief Gets the module at position @p _idx in ::modules.
		@param _idx The position of the module to retrieve from ::modules.
		@returns The reference to the shared pointer encapsulating the target module.
		*/
		inline const std::shared_ptr<ECellEngine::Data::Module>& GetModule(const std::size_t& _idx) const
		{
			return modules[_idx];
		}

		/*!
		@brief Gets the list of modules imported in this simulation (::modules).
		@returns The list of all modules.
		*/
		inline const std::vector<std::shared_ptr<ECellEngine::Data::Module>>& GetModules() const noexcept
		{
			return modules;
		}

		/*!
		@brief Gets the solver at position @p _idx in ::solvers.
		@param _idx The position of the solver to retrieve from ::solvers.
		@returns The reference to the shared pointer encapsulating the target solver.
		*/
		inline const std::shared_ptr<Solver>& GetSolver(const std::size_t& _idx) const
		{
			return solvers[_idx];
		}

		/*!
		@brief Gets the list of solvers added in this simulation (::solvers).
		@returns The list of all solvers.
		*/
		inline const std::vector<std::shared_ptr<Solver>>& GetSolvers() const noexcept
		{
			return solvers;
		}

		/*
		@brief Gets the timer of this simulation.
		*/
		inline Timer* GetTimer() noexcept
		{
			return &timer;
		}

		/*
		@brief Finds the iterator of the module which ID matches the argument
				@p _moduleID.
		@details Uses a binary search from the start to the end of the list of ::modules.
		@param _moduleID The ID of the module which iterator to look for.
		@returns The iterator of the first encounterd module which ID matches
				 the argument @p _moduleID. Returns @a SIZE_MAX if no match
				 was found.
		*/
		const std::vector<std::shared_ptr<Data::Module>>::iterator FindModule(const std::size_t _moduleID);

		/*!
		brief Finds the pointer to the first pair <moduleIdx, solverIdx> in ::moduleSolverLinks
			  which moduleIdx is less than @p _moduleIdx and solverIdx is less than @p _solverIdx.
		@details Uses 3 binary searches: two to find the range (lower & upper bounds)
				 of modules with the same ID as @p _moduleIdx and one to find
				 the solver with ID @p _solverIdx in the range found by the
				 first two searches.
				 
				 This is possible because ::moduleSolverLinks
				 is sorted on first the moduleIdx and then the solverIdx.
		@param _moduleIdx The index of the module in ::modules which is part of the pair
						  to look for in ::moduleSolverLinks.
		@param _solverIdx The index of the solver in ::solvers which is part of the pair
						  to look for in ::moduleSolverLinks.
		@returns The pointer to the first pair <moduleIdx, solverIdx> in ::moduleSolverLinks
				 which moduleIdx is less than @p _moduleIdx and solverIdx is less
				 than @p _solverIdx. Returns @a ::moduleSolverLinks.end() if no
				 match was found.
		*/
		const std::vector<std::pair<std::size_t, std::size_t>>::iterator FindModuleSolverLinkLB(const std::size_t _moduleIdx, const std::size_t _solverIdx);

		/*
		@brief Finds the iterator of the solver which ID matches the argument
				@p _solverID.
		@details Uses a binary search from the start to the end of the list of ::solvers.
		@param _solverID The ID of the solver which iterator to look for.
		@returns The iterator of the first encounterd solver which ID matches
				 the argument @p _solverID. Returns @a SIZE_MAX if no match
				 was found.
		*/
		const std::vector<std::shared_ptr<Solvers::Solver>>::iterator FindSolver(const std::size_t _solverID);

		/*!
		@brief Computes all the dependencies between the datastructures loaded
				in this simulation.
		*/
		inline void RefreshDependenciesDatabase() noexcept
		{
			dependenciesDB.RefreshDependencies(dataState);
		}

		/*!
		@brief Removes (destroy) the module with ID @p _id in ::modules
		@details This is "easy" because ::moduleSolverLinks is sorted on the module 
				 index first. So, we can get the range (2 binary searches) where the
				 module is in ::moduleSolverLinks and remove all the pairs in one go.
		@param _id The ID of the module to remove from ::modules.
		*/
		void RemoveModule(const std::size_t _id);

		/*!
		@brief Removes the pair <moduleID, solverID> from ::moduleSolverLinks
		@param _moduleID The ID of the module in the pair to remove.
		@param _solverID The ID of the solver in the pair to remove.
		@return @a True if the pair was found and removed, @a False otherwise.
		*/
		bool RemoveModuleSolverLink(const std::size_t _moduleID, const std::size_t _solverID);

		/*!
		@brief Removes (destroy) the solver with ID @p _id in ::solvers
		@details This is more "complicated" than ::RemoveModule(const std::size_t _id).
				 because ::moduleSolverLinks is sorted on the module index first and then
				 the solver index. So, we cannot find a range of pairs to remove in one go.
				 Rather, we need to iterate over the list of ::moduleSolverLinks and remove
				 them one by one. This can be done with k binary searches where k is the
				 number of modules linked to the solver we want to remove. Each binary search
				 will be on a smaller range than the previous one as we move forward.
		@param _id The ID of the solver to remove from ::solvers.
		*/
		void RemoveSolver(const std::size_t _id);

		/*!
		@brief API to execute code once before the simulation's update loop.
		*/
		void Start();

		/*!
		@brief Tries to link a solver to a module.
		@details Overrides any previous link the solver and module may have with
				 ofther module or solver respectively.
		@param _moduleID The ID of the module in ::modules we try to attach to.
		@param _solverID The ID of the solver in ::solvers to try to attach.
		*/
		void TryModuleSolverLink(const std::size_t& _moduleID, const std::size_t& _solverID);

		/*!
		@brief Updates one step of duration @p _deltaTime for every linked solver
				of the simulation.
		@param _deltaTime The duration to simulate.
		*/
		void Update(const float& _deltaTime);
	};
}