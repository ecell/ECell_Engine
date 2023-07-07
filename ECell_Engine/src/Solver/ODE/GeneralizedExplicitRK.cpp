#include "Solver/ODE/GeneralizedExplicitRK.hpp"
#include "Core/Watcher.hpp"

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

	extEqSize = dataState.GetEquations().size();
	externalEquations.reserve(extEqSize);
	for (auto [equationName, equation] : dataState.GetEquations())
	{
		externalEquations.push_back(equation.get());
	}

	ScanForWatchersOnODE();
	ScanForWatchersOnExtEq();

	//SetToClassicRK4();
	SetToDormandPrince5();
	//SetToMerson4();
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::ScanForWatchersOnExtEq() noexcept
{
	//Compare watchers and equations to see if there is any
	//watcher to account for when solving the system
	std::string equationName;
	std::string watcherTargetName;
	std::string watcherThresholdName;
	for (std::vector<std::shared_ptr<Core::Watcher<Operand*, Operand*>>>::const_iterator it = dataState.GetWatchers().begin();
		it != dataState.GetWatchers().end(); ++it)
	{
		watcherTargetName = it->get()->GetTarget()->name;
		watcherThresholdName = it->get()->GetThreshold()->name;
		for (unsigned short i = 0; i < extEqSize; i++)
		{
			equationName = externalEquations[i]->GetName();
			if (watcherTargetName == equationName || watcherThresholdName == equationName)
			{
				ECellEngine::Logging::Logger::GetSingleton().LogDebug("Watcher involving target " +
					watcherTargetName + " and " + watcherThresholdName + " was found to match variable " +
					equationName + " in the External equations.");

				watchersOnExtEq.push_back(std::pair(it->get(), i));
			}
		}
	}
}

void ECellEngine::Solvers::ODE::GeneralizedExplicitRK::ScanForWatchersOnODE() noexcept
{
	//Compare watchers and equations to see if there is any
	//watcher to account for when solving the system
	std::string systemVariableName;
	std::string watcherTargetName;
	std::string watcherThresholdName;
	for (std::vector<std::shared_ptr<Core::Watcher<Operand*, Operand*>>>::const_iterator it = dataState.GetWatchers().begin();
		it != dataState.GetWatchers().end(); ++it)
	{
		watcherTargetName = it->get()->GetTarget()->name;
		watcherThresholdName = it->get()->GetThreshold()->name;
		for (unsigned short i = 0; i < systemSize; i++)
		{
			systemVariableName = system[i].GetOperand()->name;
			if (watcherTargetName == systemVariableName || watcherThresholdName == systemVariableName)
			{
				ECellEngine::Logging::Logger::GetSingleton().LogDebug("Watcher involving target " + 
					watcherTargetName + " and " + watcherThresholdName + " was found to match variable " +
					systemVariableName + " in the ODEs");

				watchersOnODE.push_back(std::pair(it->get(), i));
			}
		}
	}
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
	yn_ext = new float[extEqSize];

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
	yn_ext = new float[extEqSize];

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
	yn_ext = new float[extEqSize];

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
		for (unsigned short i = 0; i < extEqSize; ++i)
		{
			yn_ext[i] = externalEquations[i]->Get();
		}

		for (unsigned short s = 2; s < coeffs.stages+1; ++s)
		{
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("---- s= " + std::to_string(s) + "; ");
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				system[i].GetOperand()->Set(yn[i] + stepper.h_next * coeffs.ComputekSumForStage(i * systemSize, s));
			}
			//updating the external equations with the intermediate value
			for (unsigned short i = 0; i < extEqSize; ++i)
			{
				externalEquations[i]->Compute();
			}
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				coeffs.ks[i * systemSize + s-1] = system[i].GetOperation().Get();
				//ECellEngine::Logging::Logger::GetSingleton().LogDebug("k" + std::to_string(s) + "[" + std::to_string(i) + "] = " + std::to_string(coeffs.ks[i * systemSize + s - 1]));
			}
		}

		//reset of the external equations values to their previous values at t=tn
		//before calculating y_n+1
		for (unsigned short i = 0; i < extEqSize; ++i)
		{
			externalEquations[i]->GetOperand()->Set(yn_ext[i]);
		}
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//y_n+1 = y_n + h * (b1 * k1 + b2 * k2 + ... + bs * ks)
			system[i].GetOperand()->Set(yn[i] + stepper.h_next * coeffs.ComputekSumForSolution(i * systemSize, coeffs.bs));
		}

		//Finally, we update the external equations with the new value of the system at t=tn+1
		//when every y_n+1 have been calculated.
		for (unsigned short i = 0; i < extEqSize; ++i)
		{
			externalEquations[i]->Compute();
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
		for (unsigned short i = 0; i < extEqSize; ++i)
		{
			yn_ext[i] = externalEquations[i]->Get();
		}

		for (unsigned short s = 2; s < coeffs.stages+1; ++s)
		{
			//ECellEngine::Logging::Logger::GetSingleton().LogDebug("---- s= " + std::to_string(s) + "; ");
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				system[i].GetOperand()->Set(yn[i] + stepper.h_next * coeffs.ComputekSumForStage(i * systemSize, s));
			}
			//updating the external equations with the intermediate value
			for (unsigned short i = 0; i < extEqSize; ++i)
			{
				externalEquations[i]->Compute();
			}
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				coeffs.ks[i * systemSize + s-1] = system[i].GetOperation().Get();
				//ECellEngine::Logging::Logger::GetSingleton().LogDebug("k" + std::to_string(s) + "[" + std::to_string(i) + "] = " + std::to_string(coeffs.ks[i * systemSize + s - 1]));
			}
		}

		//reset of the external equations values to their previous values at t=tn
		//before calculating y_n+1
		for (unsigned short i = 0; i < extEqSize; i++)
		{
			externalEquations[i]->GetOperand()->Set(yn_ext[i]);
		}
		
		//We update the system's buffer with the two solutions for value of y_n+1
		for (unsigned short i = 0; i < systemSize; ++i)
		{
			//y_n+1 = y_n + h * (b1 * k1 + b2 * k2 + ... + bs * ks)
			ynp1[i] = yn[i] + stepper.h_next * coeffs.ComputekSumForSolution(i * systemSize, coeffs.bs);
			ynp12[i] = yn[i] + stepper.h_next * coeffs.ComputekSumForSolution(i * systemSize, coeffs.bs2);
		}

		//If the error is acceptable
		if (stepper.ComputeError(yn, ynp1, ynp12, systemSize))
		{
			watcherTriggerTime = stepper.t + 2*stepper.h_next;//just make sure watcherTimeTrigger > stepper.t + stepper.h_next
			float watcherCandidateTime;

			//We start checking if any watcher using values modified in the ODE system
			//must be triggered
			for (std::vector<std::pair<Core::Watcher<Operand*, Operand*>*, unsigned short>>::const_iterator it = watchersOnODE.begin();
				it != watchersOnODE.end(); ++it)
			{
				//We update the variables
				system[it->second].GetOperand()->Set(ynp1[it->second]);

				//We check if the watcher is verified with the new value
				if (it->first->IsConditionNewlyVerified())
				{
					watcherCandidateTime = stepper.ComputeTimeForValue(
						it->first->GetThreshold()->Get(), yn[it->second], ynp1[it->second],
						coeffs.bsp, coeffs.ks, it->second * systemSize, coeffs.order, coeffs.stages);

					/*ECellEngine::Logging::Logger::GetSingleton().LogDebug("The watcher comparing " +
						it->first->GetTarget()->name + " and " + it->first->GetThreshold()->name +
						" should be triggered at time: " + std::to_string(watcherCandidateTime));*/

					//update the value of watcherTriggerTime & watcher
					//if the watcher trigger time is before the current
					//watcher trigger time. Goal is to find the earliest
					//watcher to trigger
					if (watcherCandidateTime < watcherTriggerTime)
					{
						watcherTriggerTime = watcherCandidateTime;
						watcher = it->first;
					}
				}
			}

			//Then we check if any watcher using values modified in the external equations
			//must be triggered
			for (std::vector<std::pair<Core::Watcher<Operand*, Operand*>*, unsigned short>>::const_iterator it = watchersOnExtEq.begin();
				it != watchersOnExtEq.end(); ++it)
			{
				//we set the system to the values at the end of the step
				for (unsigned short i = 0; i < systemSize; ++i)
				{
					system[i].GetOperand()->Set(ynp1[i]);
				}
				//we compute the external equations with the value of
				//the end of the step
				for (unsigned short i = 0; i < extEqSize; ++i)
				{
					externalEquations[i]->Compute();
				}

				//We check if the watcher is verified with the new value
				//We do the dichitomy here and not in the stepper because we need the value of the
				//external equations for this. It's really not the best place to do it but it's
				//the easiest way for now.
				// Probably the issue stems with the necessity to update the external equations
				// at the same time as the ODE system. I really need to find a way to better
				// handle this dependencies. A good way to start could be to compile the 
				// dependencies of variables. But there is the issue of maintaining the dependencies
				// when the user adds/removes equations in the future at runtime.
				if (it->first->IsConditionNewlyVerified())
				{
					float deltaTarget = fabsf(it->first->GetThreshold()->Get() - yn_ext[it->second]);
					float deltaStep = fabsf(externalEquations[it->second]->Get() - yn_ext[it->second]);
					float a = 0;
					float b = 1;

					float theta = 0.5f;
					//Interpolating the system at theta = 0.5 for the initialization
					//of the dichotomy
					for (unsigned short i = 0; i < systemSize; i++)
					{
						system[i].GetOperand()->Set(yn[i] + stepper.h_next * stepper.ComputeDenseOutputIncrement(coeffs.bsp, theta, coeffs.ks, i * systemSize, coeffs.order, coeffs.stages));
					}
					//we compute the external equations with the value of
					//of the system at theta = 0.5
					for (unsigned short i = 0; i < extEqSize; ++i)
					{
						externalEquations[i]->Compute();
					}

					float deltaTheta = fabsf(externalEquations[it->second]->Get() - yn_ext[it->second]);

					while (fabsf(deltaTarget - deltaTheta) / deltaStep > stepper.computeTimeThetaTolerance)
					{
						if (deltaTarget < deltaTheta)
						{
							b = theta;
						}
						else
						{
							a = theta;
						}
						theta = (a + b) * 0.5f;

						//Interpolating the system at theta
						for (unsigned short i = 0; i < systemSize; i++)
						{
							system[i].GetOperand()->Set(yn[i] + stepper.h_next * stepper.ComputeDenseOutputIncrement(coeffs.bsp, theta, coeffs.ks, i * systemSize, coeffs.order, coeffs.stages));
						}
						//we compute the external equations with the value of
						//of the system at theta
						for (unsigned short i = 0; i < extEqSize; ++i)
						{
							externalEquations[i]->Compute();
						}
						deltaTheta = fabsf(externalEquations[it->second]->Get() - yn_ext[it->second]);
					}

					watcherCandidateTime = stepper.t + theta * stepper.h_next;

					/*ECellEngine::Logging::Logger::GetSingleton().LogDebug("The watcher comparing " +
						(*it)->GetTarget()->name + " and " + (*it)->GetThreshold()->name +
						" should be triggered at time: " + std::to_string(watcherCandidateTime));*/

					//update the value of watcherTriggerTime & watcher
					//if the watcher trigger time is before the current
					//watcher trigger time. Goal is to find the earliest
					//watcher to trigger
					if (watcherCandidateTime < watcherTriggerTime)
					{
						watcherTriggerTime = watcherCandidateTime;
						watcher = it->first;
					}
				}
			}

			//if the watcher trigger time is after the current time,
			//it means that we found a watcher that must be triggered
			if (stepper.NextGE(watcherTriggerTime))
			{
				/*ECellEngine::Logging::Logger::GetSingleton().LogDebug("--- TRIGGERING WATCHER ---");
				ECellEngine::Logging::Logger::GetSingleton().LogDebug(" Processing watcher of target: " +
					watcher->GetTarget()->name + " at time: " + std::to_string(watcherTriggerTime));*/

				//Then, we need to update the system to the time at which
				//the watcher must be triggered (by interpolation)
				float theta = stepper.ComputeDenseOutputTime(watcherTriggerTime, stepper.t, stepper.t + stepper.h_next);
				for (unsigned short i = 0; i < systemSize; ++i)
				{
					ynp1[i] = yn[i] + stepper.h_next * stepper.ComputeDenseOutputIncrement(coeffs.bsp, theta, coeffs.ks,
						i * systemSize, coeffs.denseOutputOrder, coeffs.stages);
					system[i].GetOperand()->Set(ynp1[i]);
				}

				//we compute the external equations with the value of
					//of the system at theta = 0.5
				for (unsigned short i = 0; i < extEqSize; ++i)
				{
					externalEquations[i]->Compute();
				}

				//We call all events associated to the watcher
				watcher->CallEvents();

				//We update the stepper to the time at which the watcher was triggered
				stepper.ForceNext(watcherTriggerTime - stepper.t);
			}

			//if no watcher to trigger was found, then we continue the integration
			else
			{
				//Value Debugging
				ECellEngine::Logging::Logger::GetSingleton().LogDebug("--- ACCEPTED ---");

				//We update the system with the new value of y_n+1
				for (unsigned short i = 0; i < systemSize; ++i)
				{
					system[i].GetOperand()->Set(ynp1[i]);
				}
				//Finally, we update the external equations with the new value of the system at t=tn+1
				//when every y_n+1 have been calculated.
				for (unsigned short i = 0; i < extEqSize; i++)
				{
					externalEquations[i]->Compute();
				}

				//we advance time by the current step
				stepper.Next();

				//we compute the next step size
				stepper.ComputeNext(coeffs.estimationsMinOrder);
			}
		}
		else
		{
			//Value Debugging
			ECellEngine::Logging::Logger::GetSingleton().LogDebug("--- REJECTED ---");

			//We reset the system to the values at the beginning of the step
			for (unsigned short i = 0; i < systemSize; ++i)
			{
				ynp1[i] = yn[i];
				system[i].GetOperand()->Set(yn[i]);
			}

			for (unsigned short i = 0; i < extEqSize; i++)
			{
				externalEquations[i]->GetOperand()->Set(yn_ext[i]);
			}

			stepper.ComputeNext(coeffs.estimationsMinOrder);
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