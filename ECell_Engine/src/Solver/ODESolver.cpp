#include "Solver/ODESolver.hpp"

void ECellEngine::Solvers::ODESolver::BuildEquationRHS(Operation& _outRHS, std::vector<ECellEngine::Maths::Operation>& _fluxes)
{
	//If only one in-flux, then there is no need to sum
	if (_fluxes.size() == 1)
	{
		_outRHS.AddOperation(_fluxes[0]);
	}
	else
	{
		//We create the sum of all in-flux by summing by pairs
		//of in-flux until there is only one left.
		unsigned short halfSize = _fluxes.size() / 2;
		for (unsigned short step = 1; step <= halfSize; step *= 2)
		{
			Operation couple;
			couple.Set(&ECellEngine::Maths::functions.add);
			for (unsigned short i = 0; i < _fluxes.size(); i += step * 2)
			{
				couple.AddOperation(_fluxes[i]);
				couple.AddOperation(_fluxes[i + step]);
				_fluxes[i] = couple;
			}
		}
		//By the end of the for loop, the first element of _fluxes will be
		//the sum of all the elements.
		_outRHS.AddOperation(_fluxes[0]);
	}
}

void ECellEngine::Solvers::ODESolver::Initialize(const ECellEngine::Data::Module* _module)
{
	BiochemicalSolver::Initialize(_module);

	system.clear();
	system.reserve(dataState.GetAllSpecies().size());

	//Retrieve all the kinetic laws affecting species and differentiating
	//in-flux (when species is produced) from out-flux (when species is
	//consumed)
	std::unordered_map<std::string, std::vector<Operation>> allInFlux;
	std::unordered_map<std::string, std::vector<Operation>> allOutFlux;
	for (auto [reactionName, reaction] : dataState.GetReactions())
	{
		for (std::string reactant : *reaction->GetReactants())
		{
			//Get the kinetic law
			allOutFlux[reactant].push_back(reaction->GetKineticLaw());
		}


		for (std::string product : *reaction->GetProducts())
		{
			//Get the kinetic law
			allInFlux[product].push_back(reaction->GetKineticLaw());
		}
	}

	//Loop over all species and create the differential equation
	//as the sum of all in-flux minus the sum of all out-flux if
	//the species was indeed affected by a reaction (cf. above)
	for (auto [speciesName, species] : dataState.GetAllSpecies())
	{
		ECellEngine::Logging::Logger::GetSingleton().LogDebug(speciesName);
		//Initialize the operation in-flux minus out-flux
		auto IFSearch = allInFlux.find(speciesName);
		auto OFSearch = allOutFlux.find(speciesName);

		bool hasIF = IFSearch != allInFlux.end();
		bool hasOF = OFSearch != allOutFlux.end();

		if (hasIF || hasOF)
		{
			Operation rhs;
			if (hasIF && !hasOF)
			{
				rhs.Set(&ECellEngine::Maths::functions.identity);
				BuildEquationRHS(rhs, IFSearch->second);
			}
			else if (!hasIF && hasOF)
			{
				rhs.Set(&ECellEngine::Maths::functions.times);
				rhs.AddNewConstant(-1);
				BuildEquationRHS(rhs, OFSearch->second);
			}
			else
			{
				rhs.Set(&ECellEngine::Maths::functions.minus);
				BuildEquationRHS(rhs, IFSearch->second);
				BuildEquationRHS(rhs, OFSearch->second);
			}

			rhs.LinkLocalOperands();
		
			system.emplace_back(Maths::Equation(species.get(), rhs));
			ECellEngine::Logging::Logger::GetSingleton().LogDebug(rhs.ToString());
		}
	}

	delete[] k1;
	delete[] k2;
	delete[] k3;
	delete[] k4;
	delete[] yn;

	nbEquations = system.size();
	k1 = new float[nbEquations];
	k2 = new float[nbEquations];
	k3 = new float[nbEquations];
	k4 = new float[nbEquations];
	yn = new float[nbEquations];
}

void ECellEngine::Solvers::ODESolver::Update(const ECellEngine::Core::Timer& _timer)
{
	//implementation of the Runge-Kutta 4th order method
    //https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

	while (solveCurrentTime + solveDeltaTime < _timer.elapsedTime)
	{

		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			//y_n
			yn[i] = system[i].Get();

			//k1 = f(y_n)
			k1[i] = system[i].GetOperation().Get();
		}

		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			//k2 = f(y_n + 0.5 * dt * k1)
			system[i].GetOperand()->Set(yn[i] + halfSolveDeltaTime * k1[i]);
		}
		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			k2[i] = system[i].GetOperation().Get();
		}

		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			//k3 = f(y_n + 0.5 * dt * k2)
			system[i].GetOperand()->Set(yn[i] + halfSolveDeltaTime * k2[i]);
		}
		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			k3[i] = system[i].GetOperation().Get();
		}

		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			//k4 = f(y_n + dt * k3)
			system[i].GetOperand()->Set(yn[i] + solveDeltaTime * k3[i]);
		}
		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			k4[i] = system[i].GetOperation().Get();
		}

		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			//y_n+1 = y_n + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
			system[i].GetOperand()->Set(yn[i] + oneSixthSolveDeltaTime * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]));
		}

		solveCurrentTime += solveDeltaTime;

		std::string log = "t= " + std::to_string(solveCurrentTime) + "; ";
		for (unsigned short i = 0; i < nbEquations; ++i)
		{
			log += + "; " + system[i].GetOperand()->name + "=" + std::to_string(system[i].Get());
		}

		/*ECellEngine::Logging::Logger::GetSingleton().LogDebug(
			"t= " + std::to_string(solveCurrentTime) + "; " +
			"yn+1=" + std::to_string(system[0].Get()) + "; " +
			"k1=" + std::to_string(k1[0]) + "; " +
			"k2=" + std::to_string(k2[0]) + "; " +
			"k3=" + std::to_string(k3[0]) + "; " +
			"k4=" + std::to_string(k4[0]));*/
		ECellEngine::Logging::Logger::GetSingleton().LogDebug(log);

	}
}