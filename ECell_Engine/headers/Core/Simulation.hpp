#pragma once

#include <filesystem> //for std::filesystem::path
#include <memory> //for std::shared_ptr
#include <vector> //for std::vector<>

#include "Data/DependenciesDatabase.hpp"
#include "IO/ModuleImporterManager.hpp"
#include "Solver/GillespieNRMRSolver.hpp"

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
		@warning For the search API ::FindModuleIdx(const char* _moduleName) to
				 work accordingly, the number of module in this list must be less
				 or equal to SIZE_MAX-1 (the highest value of std::size_t). This
				 is: modules.size <= SIZE_MAX-1
		*/
		std::vector<std::shared_ptr<ECellEngine::Data::Module>> modules;
		
		/*!
		@brief The list of added solvers in the simulation space.
		@remarks A solver added to a simulation but not attached to any solver
				 will have no impact on the simulation.
		@warning For the search API ::FindSolverIdx(const char* _solverName) to
				 work accordingly, the number of solver in this list must be less
				 or equal to SIZE_MAX-1 (the highest value of std::size_t). This
				 is: solvers.size <= SIZE_MAX-1
		*/
		std::vector<std::shared_ptr<Solver>> solvers;

		/*!
		@brief The list of models assets which have a solver attached to them.
		@details  First item is the index to find the module in the vector ::modeuls.
				  Second item is the index to find the module in the vector ::solvers.
		*/
		std::vector<std::pair<std::size_t, std::size_t>> modulesToSolversTable;

		/*!
		@brief Scans to find if either the module at index @p _modIdx in ::modules or
				the solver at index @p _solverIdx in ::solvers are present in
				::modulesToSolversTable.
		@returns A pair of search reasults. At the first position is the search
				 result for the module; at the second position is the search result
				 for the solver. The search results are pairs as well with a boolean
				 at the first position and an idex at the second position. If the
				 boolean is TRUE, then we found a module (or solver) and the position
				 of indicated by the index.
		*/
		const std::pair<std::pair<bool, std::size_t>, std::pair<bool, std::size_t>> ScanModuleToSolverTable(const std::size_t& _solverIdx, const std::size_t& _modIdx);

		/*!
		@brief The simulation's timing information
		@Details Namely, contains information about the deltaTime of the simulation.
		*/
		Timer timer;

	public:
		Simulation() = default;

		/*!
		@brief Tries to add the file at path @p _filePath as an asset to the simulation.
		@param _filePath The directory where to look for the target file. This is the 
						 complete directory with the name the file at the end.
		@remaks This method is accessible from outside via the command
				ECellEngine::IO::AddModuleCommand.
		*/
		void AddModule(const std::string& _filePath);

		/*!
		@brief Adds a solver of type indicated by @p _solverClassName.
		@param _solverClassName The name of the type of solver to add. It must
								match a name of class deriving from
								ECellEngine::Solvers::Solver.
		*/
		void AddSolver(const std::string& _solverClassName);

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
		@brief Finds the module which name matches the argument @p _moduleName.
		@details Implements a naive linear search from the start of the list
				 towards the end.
		@param _moduleName the name of the module to look for.
		@returns The pointer to the first encounterd module which name matches
				 the argument @p _moduleName. Returns @a nullptr if no match
				 was found.
		*/
		ECellEngine::Data::Module* FindModule(const char* _moduleName);

		/*
		@brief Finds the index of the module which name matches the argument
				@p _moduleName.
		@details Implements a naive linear search from the start of the list
				 towards the end.
		@param _moduleName the name of the module which index to look for.
		@returns The index of the first encounterd module which name matches
				 the argument @p _moduleName. Returns @a SIZE_MAX if no match
				 was found.
		*/
		const std::size_t FindModuleIdx(const char* _moduleName);

		/*
		@brief Finds the solver which name matches the argument @p _solverName.
		@details Implements a naive linear search from the start of the list
				 towards the end.
		@param _solverName the name of the solver to look for.
		@returns The pointer to the first encounterd solver which name matches
				 the argument @p _solverName. Returns @a nullptr if no match
				 was found.
		*/
		ECellEngine::Solvers::Solver* FindSolver(const char* _solverName);

		/*
		@brief Finds the index of the solver which name matches the argument
				@p _solverName.
		@details Implements a naive linear search from the start of the list
				 towards the end.
		@param _solverName the name of the solver which index to look for.
		@returns The index of the first encounterd solver which name matches
				 the argument @p _solverName. Returns @a SIZE_MAX if no match
				 was found.
		*/
		const std::size_t FindSolverIdx(const char* _solverName);

		/*!
		@brief Computes all the dependencies between the datastructures loaded
				in this simulation.
		*/
		inline void RefreshDependenciesDatabase() noexcept
		{
			dependenciesDB.RefreshDependencies(dataState);
		}

		/*!
		@brief Removes (destroy) the module at position @p _idx in ::modules
		@param _idx The position of the module to remove from ::modules.
		@todo Definition.
		*/
		void RemoveModule(const std::size_t& _idx);

		/*!
		@brief Removes (destroy) the solver at position @p _idx in ::solvers
		@param _idx The position of the solver to remove from ::solvers.
		@todo Definition.
		*/
		void RemoveSolver(const std::size_t& _idx);

		/*!
		@brief Tries to link a solver to a module.
		@detials Overrides any previous link the solver and module may have with
				 ofther module or solver respectively.
		@param _solverIdx The position of the solver in ::solvers to try to attach.
		@param _moduleIdx The position of the module in ::modules we try to attach to.
		*/
		void TryAttachSolverToModule(const std::size_t& _solverIdx, const std::size_t& _moduleIdx);

		/*!
		@brief Updates one step of duration @p _deltaTime for every linked solver
				of the simulation.
		@param _deltaTime The duration to simulate.
		*/
		void Update(const float& _deltaTime);
	};
}