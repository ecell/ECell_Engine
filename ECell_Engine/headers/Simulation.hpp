#pragma once

#include <filesystem> //for std::filesystem::path
#include <memory> //for std::shared_ptr
#include <iterator>
#include <vector> //for std::vector<>

#include "Module.hpp"
#include "ModuleImporterManager.hpp"

#include "Solver.hpp"
#include "GillespieNRMRSolver.hpp"

using namespace ECellEngine::Data;
using namespace ECellEngine::IO;
using namespace ECellEngine::Solvers;

namespace ECellEngine::Core
{
	class Simulation
	{
	private:
		DataState dataState;
		ModuleImporterManager moduleImporterManager;

		std::vector<std::shared_ptr<Module>> modules;
		std::vector<std::shared_ptr<Solver>> solvers;
		std::vector<std::pair<std::size_t, std::size_t>> modulesToSolversTable;

		const std::pair<std::pair<bool, std::size_t>, std::pair<bool, std::size_t>> ScanModuleToSolverTable(const std::size_t& _solverIdx, const std::size_t& _modIdx);

	public:
		Simulation() = default;

		void AddModule(const std::string& _filePath);
		void AddSolver(const std::string& _solverClassName);

		inline const std::shared_ptr<Module>& GetModule(const std::size_t& _idx) const
		{
			return modules[_idx];
		}

		inline const std::vector<std::shared_ptr<Module>>& GetModules() const noexcept
		{
			return modules;
		}

		inline const std::shared_ptr<Solver>& GetModules(const std::size_t& _idx) const
		{
			return solvers[_idx];
		}

		inline const std::vector<std::shared_ptr<Solver>>& GetSolvers() const noexcept
		{
			return solvers;
		}

		void RemoveModule(const std::size_t& _idx);

		void RemoveSolver(const std::size_t& _idx);

		void TryAttachSolverToModule(const std::size_t& _solverIdx, const std::size_t& _moduleIdx);

		void Update(const float& _deltaTime);
	};
}