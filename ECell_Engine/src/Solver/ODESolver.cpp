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
	delete[] yn_ext;

	systemSize = system.size();
	k1 = new float[systemSize];
	k2 = new float[systemSize];
	k3 = new float[systemSize];
	k4 = new float[systemSize];
	yn = new float[systemSize];

	externalEquations = &dataState.GetEquations();
	yn_ext = new float[externalEquations->size()];
}

void ECellEngine::Solvers::ODESolver::Update(const ECellEngine::Core::Timer& _timer)
{
	//implementation of the Runge-Kutta 4th order method
    //https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

	while (solveCurrentTime + solveDeltaTime < _timer.elapsedTime)
	{
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//y_n
			yn[i] = system[i].Get();

			//k1 = f(y_n)
			k1[i] = system[i].GetOperation().Get();
		}

		//Storing the value of the external equations at the beginning of the step
		unsigned short j = 0;
		for (auto [equationName, equation] : *externalEquations)
		{
			yn_ext[j] = equation->Get();
			j++;
		}

		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//k2 = f(y_n + 0.5 * dt * k1)
			system[i].GetOperand()->Set(yn[i] + halfSolveDeltaTime * k1[i]);
		}
		//updating the external equations with the intermediate value
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			equation->Compute();
		}
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			k2[i] = system[i].GetOperation().Get();
		}

		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//k3 = f(y_n + 0.5 * dt * k2)
			system[i].GetOperand()->Set(yn[i] + halfSolveDeltaTime * k2[i]);
		}
		//updating the external equations with the intermediate value
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			equation->Compute();
		}
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			k3[i] = system[i].GetOperation().Get();
		}

		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//k4 = f(y_n + dt * k3)
			system[i].GetOperand()->Set(yn[i] + solveDeltaTime * k3[i]);
		}
		//updating the external equations with the intermediate value
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			equation->Compute();
		}
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			k4[i] = system[i].GetOperation().Get();
		}

		//reset of the external equations values to their previous values at t=tn
		//before calculating y_n+1
		j = 0;
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			equation->GetOperand()->Set(yn_ext[j]);
			j++;
		}
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//y_n+1 = y_n + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
			system[i].GetOperand()->Set(yn[i] + oneSixthSolveDeltaTime * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]));
		}

		//Finally, we update the external equations with the new value of the system at t=tn+1
		//when every y_n+1 have been calculated.
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			equation->Compute();
		}

		solveCurrentTime += solveDeltaTime;


		//Value Debugging
		/*std::string log = "t= " + std::to_string(solveCurrentTime) + "; ";
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			log += + "; " + system[i].GetOperand()->name + "=" + std::to_string(system[i].Get());
		}
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			log += +"; " + equationName + "=" + std::to_string(equation->Get());
		}

		ECellEngine::Logging::Logger::GetSingleton().LogDebug(
			"t= " + std::to_string(solveCurrentTime) + "; " +
			"yn+1=" + std::to_string(system[0].Get()) + "; " +
			"k1=" + std::to_string(k1[0]) + "; " +
			"k2=" + std::to_string(k2[0]) + "; " +
			"k3=" + std::to_string(k3[0]) + "; " +
			"k4=" + std::to_string(k4[0]));

		ECellEngine::Logging::Logger::GetSingleton().LogDebug(log);*/
	}
}