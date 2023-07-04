#include "Solver/ODE/GeneralizedExplicitRK.hpp"

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::BuildEquationRHS(Operation& _outRHS, std::vector<ECellEngine::Maths::Operation>& _fluxes)
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

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::Initialize(const ECellEngine::Data::Module* _module)
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
	delete[] yn;
	delete[] ynp1;
	delete[] ynp12;
	delete[] yn_ext;

	systemSize = system.size();
	externalEquations = &dataState.GetEquations();

	//SetToClassicRK4();
	SetToDormandPrince5();
	//SetToMerson4();
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::SetToClassicRK4() noexcept
{
	errorControl = false;

	delete[] yn;
	delete[] ynp1;
	delete[] ynp12;
	delete[] yn_ext;

	yn = new float[systemSize];
	//ynp1 is not needeed in Classic because there is no error control nor interpolation
	//ynp12 is not needeed in Classic because there is no error control
	yn_ext = new float[externalEquations->size()];

	coeffs.SetToClassicRK4(systemSize);
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::SetToDormandPrince5() noexcept
{
	errorControl = true;

	delete[] yn;
	delete[] ynp1;
	delete[] ynp12;
	delete[] yn_ext;

	yn = new float[systemSize];
	ynp1 = new float[systemSize];
	//next we initialize the value of ynp1 to the current value of y
	//this is only for the initial condiions of the systems of ODEs
	for (unsigned short i = 0; i < systemSize; ++i)
	{
		ynp1[i] = system[i].Get();
	}
	ynp12 = new float[systemSize];
	yn_ext = new float[externalEquations->size()];

	coeffs.SetToDormandPrince54(systemSize);
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::SetToMerson4() noexcept
{
	errorControl = true;

	delete[] yn;
	delete[] ynp1;
	delete[] ynp12;
	delete[] yn_ext;

	yn = new float[systemSize];
	ynp1 = new float[systemSize];
	//next we initialize the value of ynp1 to the current value of y
	//this is only for the initial condiions of the systems of ODEs
	for (unsigned short i = 0; i < systemSize; ++i)
	{
		ynp1[i] = system[i].Get();
	}
	ynp12 = new float[systemSize];
	yn_ext = new float[externalEquations->size()];

	coeffs.SetToMerson4(systemSize);
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::Update(const ECellEngine::Core::Timer& _timer)
{
	if (errorControl)
	{
		UpdateWithErrorControl(_timer);
	}
	else
	{
		UpdateClassic(_timer);
	}
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::UpdateClassic(const ECellEngine::Core::Timer& _timer)
{
	//implementation of the Runge-Kutta 4th order method
	//https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

	while (stepper.NextLEQ(_timer.elapsedTime))
	{
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//y_n
			yn[i] = system[i].Get();

			//k1 = f(y_n)
			coeffs.ks[i*systemSize] = system[i].GetOperation().Get();
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("k1[" + std::to_string(i) + "] = " + std::to_string(coeffs.ks[i * systemSize]));
		}

		//Storing the value of the external equations at the beginning of the step
		unsigned short j = 0;
		for (auto [equationName, equation] : *externalEquations)
		{
			yn_ext[j] = equation->Get();
			j++;
		}

		for (unsigned short s = 2; s < coeffs.stages+1; ++s)
		{
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("---- s= " + std::to_string(s) + "; ");
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				system[i].GetOperand()->Set(yn[i] + stepper.h_next * coeffs.ComputekSumForStage(i * systemSize, s));
			}
			//updating the external equations with the intermediate value
			for (auto [equationName, equation] : dataState.GetEquations())
			{
				equation->Compute();
			}
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				coeffs.ks[i * systemSize + s-1] = system[i].GetOperation().Get();
				//ECellEngine::Logging::Logger::GetSingleton().LogDebug("k" + std::to_string(s) + "[" + std::to_string(i) + "] = " + std::to_string(coeffs.ks[i * systemSize + s - 1]));
			}
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
			//y_n+1 = y_n + h * (b1 * k1 + b2 * k2 + ... + bs * ks)
			system[i].GetOperand()->Set(yn[i] + stepper.h_next * coeffs.ComputekSumForSolution(i * systemSize, coeffs.bs));
		}

		//Finally, we update the external equations with the new value of the system at t=tn+1
		//when every y_n+1 have been calculated.
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			equation->Compute();
		}

		stepper.Next();

		//Value Debugging
		ECellEngine::Logging::Logger::GetSingleton().LogDebug("------ t= " + std::to_string(stepper.t) + "; ");
		std::string log;
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			log = "System; " + system[i].GetOperand()->name;
			log += "=" + std::to_string(system[i].Get());
			for (unsigned short j = 0; j < coeffs.stages; ++j)
			{
				log += "; k" + std::to_string(j + 1) + "=" + std::to_string(coeffs.ks[i * systemSize + j]);
			}
			ECellEngine::Logging::Logger::GetSingleton().LogDebug(log);
		}
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			ECellEngine::Logging::Logger::GetSingleton().LogDebug("Extern;" + equationName + "=" + std::to_string(equation->Get()));
		}

		ECellEngine::Logging::Logger::GetSingleton().LogDebug("------");
	}
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::UpdateWithErrorControl(const ECellEngine::Core::Timer& _timer)
{
	//implementation of the Runge-Kutta 4th order method
	//https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

	while (stepper.NextLEQ(_timer.elapsedTime))
	{
		//We initialize the buffers for this step

		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//y_n
			yn[i] = ynp1[i];
			//k1 = f(y_n)
			coeffs.ks[i*systemSize] = system[i].GetOperation().Get();
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("k1[" + std::to_string(i) + "] = " + std::to_string(coeffs.ks[i * systemSize]));
		}

		//Storing the value of the external equations at the beginning of the step
		unsigned short j = 0;
		for (auto [equationName, equation] : *externalEquations)
		{
			yn_ext[j] = equation->Get();
			j++;
		}

		for (unsigned short s = 2; s < coeffs.stages+1; ++s)
		{
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("---- s= " + std::to_string(s) + "; ");
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				system[i].GetOperand()->Set(yn[i] + stepper.h_next * coeffs.ComputekSumForStage(i * systemSize, s));
			}
			//updating the external equations with the intermediate value
			for (auto [equationName, equation] : dataState.GetEquations())
			{
				equation->Compute();
			}
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				coeffs.ks[i * systemSize + s-1] = system[i].GetOperation().Get();
				//ECellEngine::Logging::Logger::GetSingleton().LogDebug("k" + std::to_string(s) + "[" + std::to_string(i) + "] = " + std::to_string(coeffs.ks[i * systemSize + s - 1]));
			}
		}

		//reset of the external equations values to their previous values at t=tn
		//before calculating y_n+1
		j = 0;
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			equation->GetOperand()->Set(yn_ext[j]);
			j++;
		}
		
		//We update the system's buffer with the two solutions for value of y_n+1
		//We reset the values in the dataState to their previous values at t=tn
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//y_n+1 = y_n + h * (b1 * k1 + b2 * k2 + ... + bs * ks)
			ynp1[i] = yn[i] + stepper.h_next * coeffs.ComputekSumForSolution(i * systemSize, coeffs.bs);
			ynp12[i] = yn[i] + stepper.h_next * coeffs.ComputekSumForSolution(i * systemSize, coeffs.bs2);
		}


		if (stepper.ComputeError(yn, ynp1, ynp12, systemSize))
		{
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				system[i].GetOperand()->Set(ynp1[i]);
			}
			//Finally, we update the external equations with the new value of the system at t=tn+1
			//when every y_n+1 have been calculated.
			for (auto [equationName, equation] : dataState.GetEquations())
			{
				equation->Compute();
			}

			//we advance time by the current step
			stepper.Next();

			//we compute the next step size
			stepper.ComputeNext(coeffs.estimationsMinOrder);

			//Value Debugging
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("--- ACCEPTED ---");
		}
		else
		{
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				ynp1[i] = yn[i];
				system[i].GetOperand()->Set(yn[i]);
			}

			stepper.ComputeNext(coeffs.estimationsMinOrder);

			//Value Debugging
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("--- REJECTED ---");
		}

		/*ECellEngine::Logging::Logger::GetSingleton().LogDebug(
			"t= " + std::to_string(stepper.t) +
			"; error= " + std::to_string(stepper.error) + 
			"; h_prev= " + std::to_string(stepper.h_prev) +
			"; h_next= " + std::to_string(stepper.h_next));

		std::string log;
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			log = "System; " + system[i].GetOperand()->name;
			log += "=" + std::to_string(system[i].Get());
			for (unsigned short j = 0; j < coeffs.stages; ++j)
			{
				log += "; k" + std::to_string(j + 1) + "=" + std::to_string(coeffs.ks[i * systemSize + j]);
			}
			ECellEngine::Logging::Logger::GetSingleton().LogDebug(log);
		}
		for (auto [equationName, equation] : dataState.GetEquations())
		{
			ECellEngine::Logging::Logger::GetSingleton().LogDebug("Extern; " + equationName + "=" + std::to_string(equation->Get()));
		}

		ECellEngine::Logging::Logger::GetSingleton().LogDebug("------");*/
	}
}