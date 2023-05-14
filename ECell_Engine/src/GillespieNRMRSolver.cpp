#include "BiochemicalModule.hpp"
#include "GillespieNRMRSolver.hpp"

void ECellEngine::Solvers::GillespieNRMRSolver::ApplyBackward(const std::string& _reactionName)
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

void ECellEngine::Solvers::GillespieNRMRSolver::ApplyForward(const std::string& _reactionName)
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

void ECellEngine::Solvers::GillespieNRMRSolver::BuildDependancyGraph(const std::vector<std::string>& _reactions)
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

const float ECellEngine::Solvers::GillespieNRMRSolver::ComputeReactionPropensity(const std::string& _reactionName)
{
	return dataState.GetReaction(_reactionName)->ComputeKineticLaw();
}

void ECellEngine::Solvers::GillespieNRMRSolver::SolveBackward(const float& _targetTime)
{

}

void ECellEngine::Solvers::GillespieNRMRSolver::SolveForward(const float& _targetTime)
{
	//step 2 & 3
	std::pair<int, float> muTau = tauIMH.GetRoot();
	float a_new, a_old, tau_new, tau_old;
	std::string reactionName = module->GetReaction(muTau.first);
	while (muTau.second < _targetTime)
	{
		ApplyForward(reactionName);
		//ManageTrace();
		//trace.push_back(muTau.first);
		//traceBlockSize++;
		//traceSize++;

		//step 5.c: To avoid an "if" in the for loop
		//we update tau of the current reaction here.
		a_new = dataState.GetReaction(reactionName)->ComputeKineticLaw();//compute reaction propensity
		tau_new = rng.Exponential(a_new) + muTau.second;
		tauIMH.SetValueInRoot(tau_new);
		tauIMH.UpdateHeapFromRoot();

		auto depReactions = reactionsDependanceGraph.equal_range(muTau.first);
		//step 5 (*it is alpha in the algorithm)
		for (auto it = depReactions.first; it != depReactions.second; ++it)
		{
			reactionName = module->GetReaction(it->second);
			a_old = dataState.GetReaction(reactionName)->GetKineticLawValue();
			tau_old = tauIMH.GetValueAtIndex(it->second);

			//Step 5.a
			a_new = dataState.GetReaction(reactionName)->ComputeKineticLaw();//compute reaction propensity

			//Step 5.b
			tau_new = (a_old / a_new) * (tau_old - muTau.second) + muTau.second; //muTau.second is current time
			tauIMH.SetValueAtIndex(it->second, tau_new);
			tauIMH.UpdateHeapFrom(it->second, tau_old);
		}

		//we actualize step 2 & 3
		muTau = tauIMH.GetRoot();
		reactionName = module->GetReaction(muTau.first);
	}
}

void ECellEngine::Solvers::GillespieNRMRSolver::Initialize(const ECellEngine::Data::Module* _module)
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

	trace.reserve(1024);
}

void ECellEngine::Solvers::GillespieNRMRSolver::Update(const ECellEngine::Core::Timer& _timer)
{
	SolveForward(_timer.elapsedTime);
}
