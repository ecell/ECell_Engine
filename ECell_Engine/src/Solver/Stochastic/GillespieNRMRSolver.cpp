#include "Data/BiochemicalModule.hpp"
#include "Solver/Stochastic/GillespieNRMRSolver.hpp"

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::ApplyBackward(const std::string& _reactionName)
{
	ECellEngine::Data::Reaction* reaction = dataState.GetReaction(_reactionName).get();
	//Decrementing quantities of all Products by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetProducts()->begin(); it != reaction->GetProducts()->end(); it++)
	{
		dataState.GetSpecies(*it)->Decrement(1.f);
	}

	//Incrementing quantities of all reactants by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetReactants()->begin(); it != reaction->GetReactants()->end(); it++)
	{
		dataState.GetSpecies(*it)->Increment(1.f);
	}
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::ApplyForward(const std::string& _reactionName)
{
	ECellEngine::Data::Reaction* reaction = dataState.GetReaction(_reactionName).get();
	
	//Decrementing quantities of all reactants by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetReactants()->begin(); it != reaction->GetReactants()->end(); it++)
	{
		dataState.GetSpecies(*it)->Decrement(1.f);
	}

	//Incrementing quantities of all products by 1
	for (std::vector<std::string>::const_iterator it = reaction->GetProducts()->begin(); it != reaction->GetProducts()->end(); it++)
	{
		dataState.GetSpecies(*it)->Increment(1.f);
	}
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::BuildDependancyGraph(const std::vector<std::string>& _reactions)
{

	std::unordered_map<std::string, std::size_t> nameToIdx;
	size_t idx = 0;
	for (std::vector<std::string>::const_iterator itReactions = _reactions.begin(); itReactions != _reactions.end(); itReactions++)
	{
		nameToIdx.emplace(*itReactions, idx);
		idx++;
	}

	idx = 0;
	const std::vector<std::string>* species;
	for (std::vector<std::string>::const_iterator itReactions = _reactions.begin(); itReactions != _reactions.end(); itReactions++)
	{
		species = dataState.GetReaction(*itReactions)->GetProducts();
		for (std::vector<std::string>::const_iterator itSpecies = species->begin(); itSpecies != species->end(); itSpecies++)
		{
			auto reactionLinks = dataState.GetOperandsToOperations().equal_range(*itSpecies);
			for (auto itDependantReaction = reactionLinks.first; itDependantReaction != reactionLinks.second; itDependantReaction++)
			{
				auto s1 = nameToIdx.find((*itDependantReaction).second);
				if (s1 != nameToIdx.end())
				{
					reactionsDependanceGraph.emplace(idx, s1->second);
				}
			}
		}
		
		species = dataState.GetReaction(*itReactions)->GetReactants();
		for (std::vector<std::string>::const_iterator itSpecies = species->begin(); itSpecies != species->end(); itSpecies++)
		{
			auto reactionLinks = dataState.GetOperandsToOperations().equal_range(*itSpecies);
			for (auto itDependantReaction = reactionLinks.first; itDependantReaction != reactionLinks.second; itDependantReaction++)
			{
				auto s1 = nameToIdx.find((*itDependantReaction).second);
				if (s1 != nameToIdx.end())
				{
					reactionsDependanceGraph.emplace(idx, s1->second);
				}
			}
		}

		idx++;
	}
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::Clear()
{
	stepper.Reset();
	tauIMH.Clear();
	trace.clear();
	reactionsDependanceGraph.clear();
}

const float ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::ComputeReactionPropensity(const std::string& _reactionName)
{
	return dataState.GetReaction(_reactionName)->ComputeKineticLaw();
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::Reset()
{
	stepper.Reset();

	//Reset all reactions
	size_t idx = 0;
	for (std::vector<std::string>::const_iterator itr = module->GetAllReaction().begin(); itr != module->GetAllReaction().end(); itr++)
	{
		Data::Reaction* r = dataState.GetReaction(*itr).get();

		//Start by resetting all products
		for (std::vector<std::string>::const_iterator its = r->GetProducts()->begin(); its != r->GetProducts()->end(); its++)
		{
			dataState.GetSpecies(*its)->Reset();
		}

		//Then reset all reactants
		for (std::vector<std::string>::const_iterator its = r->GetReactants()->begin(); its != r->GetReactants()->end(); its++)
		{
			dataState.GetSpecies(*its)->Reset();
		}

		//Finally, compute the propensity of the reaction and update it to the heap
		tauIMH.SetValueAtIndex(idx, rng.Exponential(r->ComputeKineticLaw()));

		idx++;
	}

	//Update the heap now that every propensity has been reset
	tauIMH.UpdateHeapFromRoot();

	trace.clear();
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::SolveBackward(const float& _targetTime)
{
	//TODO: implement from previous version
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::SolveForward(const float& _targetTime)
{
	while (stepper.IsNextLE(_targetTime))
	{
		ApplyForward(stepper.reactionName);
		stepper.Next();
		//ManageTrace();
		//trace.push_back(stepper.muTau.first);
		//traceBlockSize++;
		//traceSize++;

		//step 5.c: To avoid an "if" in the for loop
		//we update tau of the current reaction here.
		stepper.a_new = dataState.GetReaction(stepper.reactionName)->ComputeKineticLaw();//compute reaction propensity
		stepper.h = rng.Exponential(stepper.a_new);
		
		tauIMH.SetValueInRoot(stepper.timer.elapsedTime + stepper.h);
		tauIMH.UpdateHeapFromRoot();

		auto depReactions = reactionsDependanceGraph.equal_range(stepper.muTau.first);
		//step 5 (*it is alpha in the algorithm)
		for (auto it = depReactions.first; it != depReactions.second; ++it)
		{
			stepper.reactionName = module->GetReaction(it->second);
			stepper.a_old = dataState.GetReaction(stepper.reactionName)->GetKineticLawValue();
			stepper.tau_old = tauIMH.GetValueAtIndex(it->second);

			//Step 5.a
			stepper.a_new = dataState.GetReaction(stepper.reactionName)->ComputeKineticLaw();//compute reaction propensity

			//Step 5.b
			stepper.ComputeDepReactionNextTime();
			tauIMH.SetValueAtIndex(it->second, stepper.tau_new);
			tauIMH.UpdateHeapFrom(it->second, stepper.tau_old);
		}

		//we actualize step 2 & 3
		stepper.muTau = tauIMH.GetRoot();
		stepper.reactionName = module->GetReaction(stepper.muTau.first);
	}
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::Initialize(const ECellEngine::Data::Module* _module)
{
	ECellEngine::Solvers::BiochemicalSolver::Initialize(_module);
	
	//Build the data structure representing the dependency graph.
	BuildDependancyGraph(module->GetAllReaction());

	//Initializes the reversible Random Number Generator
	rng.Initialize(123);

	//Draw the first reaction times (named tau in the original algorithm)
	std::vector<std::pair<std::size_t, float>> taus;
	size_t idx = 0;
	for (std::vector<std::string>::const_iterator it = module->GetAllReaction().begin(); it != module->GetAllReaction().end(); it++)
	{
		taus.push_back(std::pair<std::size_t, float>(idx, rng.Exponential(dataState.GetReaction(*it)->ComputeKineticLaw())));
		idx++;
	}

	//Builds the reaction times indexed priority queue (indexed Minheap)
	tauIMH.Initialize(taus);

	//step 2 & 3
	stepper.muTau = tauIMH.GetRoot();
	stepper.reactionName = module->GetReaction(stepper.muTau.first);

	stepper.h = stepper.muTau.second;

	trace.reserve(1024);
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::Start()
{
	stepper.Start();
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRSolver::Update(const ECellEngine::Core::Timer& _timer)
{
	SolveForward(_timer.elapsedTime);
}
