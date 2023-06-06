#include "Solver/ODESolver.hpp"

void ECellEngine::Solvers::ODESolver::Initialize(const ECellEngine::Data::Module* _module)
{
	BiochemicalSolver::Initialize(_module);

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
		//Initialize the operation in-flux minus out-flux
		Operation rhs;
		rhs.Set(&ECellEngine::Maths::functions.minus);

		//Sum all the in-flux
		auto search = allInFlux.find(speciesName);
		if (search != allInFlux.end())
		{
			std::vector<Operation>& inFlux = search->second;

			//If only one in-flux, then there is no need to sum
			if (inFlux.size() == 1)
			{
				rhs.AddOperation(inFlux[0]);
			}
			else
			{
				//We create the sum of all in-flux by summing by pairs
				//of in-flux until there is only one left.
				unsigned short halfSize = inFlux.size() / 2;
				for (unsigned short step = 1; step < halfSize; step *= 2)
				{
					Operation couple;
					couple.Set(&ECellEngine::Maths::functions.add);
					for (unsigned short i = 0; i < inFlux.size(); i += step * 2)
					{
						couple.AddOperation(inFlux[i]);
						couple.AddOperation(inFlux[i + step]);
						inFlux[i] = couple;
					}
				}
				//By the end of the for loop, the first element of inFlux will be
				//the sum of all the elements.
				rhs.AddOperation(inFlux[0]);
			}
		}
		//If no in-flux, then it is 0.
		else
		{
			rhs.AddNewConstant(0.f);
		}

		//Sum all the out-flux.
		search = allOutFlux.find(speciesName);
		if (search != allOutFlux.end())
		{
			std::vector<Operation>& outFlux = search->second;

			//If only one out-flux, then there is no need to sum.
			if (outFlux.size() == 1)
			{
				rhs.AddOperation(outFlux[0]);
			}
			else
			{
				//We create the sum of all out-flux by summing by pairs
				//of out-flux until there is only one left.
				unsigned short halfSize = outFlux.size() / 2;
				for (unsigned short step = 1; step < halfSize; step *= 2)
				{
					Operation couple;
					couple.Set(&ECellEngine::Maths::functions.add);
					for (unsigned short i = 0; i < outFlux.size(); i += step * 2)
					{
						couple.AddOperation(outFlux[i]);
						couple.AddOperation(outFlux[i + step]);
						outFlux[i] = couple;
					}
				}
				//by the end of the for loop, the first element of outFlux will be
				//the sum of all the elements.
				rhs.AddOperation(outFlux[0]);
			}
		}
		//If no out-flux, then it is 0.
		else
		{
			rhs.AddNewConstant(0.f);
		}

		rhs.LinkLocalOperands();
		
		system.emplace_back(Maths::Equation(species.get(), rhs));
	}
}

void ECellEngine::Solvers::ODESolver::Update(const ECellEngine::Core::Timer& _timer)
{
	//implementation of the Runge-Kutta 4th order method
    //https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

	//Compute the time step
	float halfdt = _timer.deltaTime / 2;
	float k1, k2, k3, k4;
	float yn;
	for (auto& equation : system)
	{
		yn = equation.Get();

		//k1 = f(y_n)
		k1 = equation.GetOperation().Get();
		
		//k2 = f(y_n + 0.5 * dt * k1)
		equation.GetOperand()->Set(yn + halfdt * k1);
		k2 = equation.GetOperation().Get();
		
		//k3 = f(y_n + 0.5 * dt * k2)
		equation.GetOperand()->Set(yn + halfdt * k2);
		k3 = equation.GetOperation().Get();
		
		//k4 = f(y_n + dt * k3)
		equation.GetOperand()->Set(yn + _timer.deltaTime * k3);
		k4 = equation.GetOperation().Get();
		
		//y_n+1 = y_n + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
		equation.GetOperand()->Set(yn + _timer.deltaTime / 6 * (k1 + 2 * k2 + 2 * k3 + k4));
	}
}