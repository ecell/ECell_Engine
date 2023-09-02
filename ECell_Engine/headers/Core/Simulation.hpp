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
		@brief The list of pairs of indeces <moduleIdx solverIdx> representing the
				link between a module and a solver.
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

		inline ECellEngine::Data::DataState& GetDataState()
		{
			return dataState;
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
		inline std::shared_ptr<ECellEngine::Data::Module>& GetModule(const std::size_t _idx)
		{
			return modules[_idx];
		}

		/*!
		@brief Gets the list of modules imported in this simulation (::modules).
		@returns The list of all modules.
		*/
		inline std::vector<std::shared_ptr<ECellEngine::Data::Module>>& GetModules() noexcept
		{
			return modules;
		}

		/*!
		@brief Gets the solver at position @p _idx in ::solvers.
		@param _idx The position of the solver to retrieve from ::solvers.
		@returns The reference to the shared pointer encapsulating the target solver.
		*/
		inline std::shared_ptr<Solver>& GetSolver(const std::size_t _idx)
		{
			return solvers[_idx];
		}

		/*!
		@brief Gets the list of solvers added in this simulation (::solvers).
		@returns The list of all solvers.
		*/
		inline std::vector<std::shared_ptr<Solver>>& GetSolvers() noexcept
		{
			return solvers;
		}

		/*
		@brief Gets the timer of this simulation.
		*/
		inline Timer& GetTimer() noexcept
		{
			return timer;
		}

		/*
		@brief Finds the iterator of the module which ID matches the argument
				@p _moduleID.
		@details Uses a binary search from the start to the end of the list of ::modules.
		@param _moduleID The ID of the module which iterator to look for.
		@returns A pair <bool iterator> where the bool indicates whether the 
				 module was found and the iterator the position at the end of
				 the search. If the module was found, bool is @a True and the
				 iterator points to the target module. Otherwise, bool is @a
				 False and the iterator points at ::modules.end().
		*/
		std::pair<bool, std::vector<std::shared_ptr<Data::Module>>::iterator> FindModule(const std::size_t _moduleID);

		/*!
		brief Gets the lower bound such that to the first pair <moduleIdx, solverIdx>
				in ::moduleSolverLinks which moduleIdx is less than @p _moduleIdx
				and solverIdx is less than @p _solverIdx.
		@details Uses 3 binary searches: two to find the range (lower & upper
				 bounds) of modules with the same ID as @p _moduleIdx and one
				 to find the solver with ID @p _solverIdx in the range found
				 by the first two searches.
				 
				 This is possible because ::moduleSolverLinks
				 is sorted on first the moduleIdx and then the solverIdx.
		@param _moduleIdx The index of the module in ::modules which is part of
						  the pair to look for in ::moduleSolverLinks.
		@param _solverIdx The index of the solver in ::solvers which is part of
						  the pair to look for in ::moduleSolverLinks.
		@returns A pair <bool iterator> where the bool indicates whether the
				 pair was found and the iterator the position at the end of the
				 search. If the pair was found, bool is @a True and the iterator
				 points to the target pair. Otherwise, bool is @a False and the
				 iterator points:
				 
				 1. at ::moduleSolverLinks.end()
					
					1.1 if no pair <moduleIdx, solverIdx> has a moduleIdx equal to
						@p _moduleIdx
					1.2 if the target moduleIdx is the greatest moduleIdx in
						::moduleSolverLinks and the target solverIdx is the greatest
						in the range of pairs with this moduleIdx.

				 2. at the first pair <moduleIdx, solverIdx> in ::moduleSolverLinks
				 with moduleIdx < @p _moduleIdx
				 
					2.1 if the search on moduleIdx failed.
					2.2 if the search on moduleIdx succeeded but the search on solverIdx
						failed.
		*/
		std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> FindModuleSolverLink(const std::size_t _moduleIdx, const std::size_t _solverIdx);

		/*
		@brief Finds the iterator of the solver which ID matches the argument
				@p _solverID.
		@details Uses a binary search from the start to the end of the list of ::solvers.
		@param _solverID The ID of the solver which iterator to look for.
		@returns The iterator of the first encounterd solver which ID matches
				 the argument @p _solverID. Returns @a SIZE_MAX if no match
				 was found.
		*/
		std::pair<bool, std::vector<std::shared_ptr<Solvers::Solver>>::iterator> FindSolver(const std::size_t _solverID);

		/*!
		@brief Computes all the dependencies between the datastructures loaded
				in this simulation.
		*/
		inline void RefreshDependenciesDatabase() noexcept
		{
			dependenciesDB.RefreshDependencies(dataState);
		}

		/*!
		@brief Erases the module at the position @p _moduleIt in ::modules as
				well as the module-solver links in ::moduleSolverLinks which
				contains the ID of the module to remove.
		@details Also maintains the integrity of ::moduleSolverLinks by decrementing
				 the moduleIdx of all the pairs <moduleIdx, solverIdx> which have
				 a moduleIdx greater than the moduleIdx of the module to remove.
		@param _moduleIt The iterator pointing to the module to remove.
		@remarks Does not check whether _moduleIt is actually in ::modules. To
				 make sure this is the case, use ::FindModule(const std::size_t _moduleID)
		*/
		void RemoveModule(std::vector<std::shared_ptr<Data::Module>>::iterator _moduleIt);

		/*!
		@brief Erases the module-solver link at the position @p _linkIt in
				::moduleSolverLinks.
		@param _linkIt The iterator pointing to the module-solver link to remove.
		*/
		void RemoveModuleSolverLink(std::vector<std::pair<std::size_t, std::size_t>>::iterator _linkIt);

		/*!
		@brief Erases the solver at the position @p _solverIt in ::solvers as
				well as the module-solver links in ::moduleSolverLinks which
				contains the ID of the solver to remove.
		@details Also maintains the integrity of ::moduleSolverLinks by decrementing
				 the solverIdx of all the pairs <moduleIdx, solverIdx> which have
				 a solverIdx greater than the solverIdx of the solver to remove.
		@param _solverIt The iterator pointing to the solver to remove.
		@remarks Does not check whether @p _solverIt is actually in ::solvers. To
				 make sure this is the case, use ::FindSolver(const std::size_t _solverID)
		*/
		void RemoveSolver(std::vector<std::shared_ptr<Solvers::Solver>>::iterator _solverIt);

		/*!
		@brief Resets all the data state and solvers of this simulation.
		@details It is not necessary to reset the modules as the data encapsulated
				 in them is reset at the same time the data state is. Solvers
				 must be reset separately however as they may contain internal
				 data based on the data state.
		*/
		void Reset() noexcept;

		/*!
		@brief API to execute code once before the simulation's update loop.
		*/
		void Start();

		/*!
		@brief Insert the module-solver link @p _link in ::moduleSolverLinks
				at the position @p _pos and initializes the solver @p _solver
				with the data of the module @p _module.
		@param _pos The position in ::moduleSolverLinks where to insert the
					module-solver link @p _link.
		@param _link The module-solver link to insert in ::moduleSolverLinks.
		@remark Does not check whether @p _pos is actually in ::moduleSolverLinks nor
				whether @p _link is actually in ::moduleSolverLinks. To make sure this
				is the case, use ::FindModuleSolverLink(const std::size_t _moduleIdx, const std::size_t _solverIdx)
		*/
		void InsertModuleSolverLink(std::vector<std::pair<std::size_t, std::size_t>>::iterator _pos,
									std::pair<std::size_t, std::size_t>& _link);

		/*!
		@brief Updates one step of duration @p _deltaTime for every linked solver
				of the simulation.
		@param _deltaTime The duration to simulate.
		*/
		void Update(const float& _deltaTime);
	};
}