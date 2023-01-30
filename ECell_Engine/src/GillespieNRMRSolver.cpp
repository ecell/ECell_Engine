#include "GillespieNRMRSolver.hpp"

void ECellEngine::Solvers::GillespieNRMRSolver::ApplyBackward(const ECellEngine::Data::Reaction& _reaction)
{
	//Decrementing quantities of all Products by 1
	for (std::vector<std::string>::const_iterator it = _reaction.GetProducts().begin(); it != _reaction.GetProducts().end(); it++)
	{
		dataState.GetSpecies(*it).Decrement(1.f);
	}

	//Incrementing quantities of all reactans by 1
	for (std::vector<std::string>::const_iterator it = _reaction.GetReactants().begin(); it != _reaction.GetReactants().end(); it++)
	{
		dataState.GetSpecies(*it).Increment(1.f);
	}
}

void ECellEngine::Solvers::GillespieNRMRSolver::ApplyForward(const ECellEngine::Data::Reaction& _reaction)
{
	//Decrementing quantities of all reactans by 1
	for (std::vector<std::string>::const_iterator it = _reaction.GetReactants().begin(); it != _reaction.GetReactants().end(); it++)
	{
		dataState.GetSpecies(*it).Decrement(1.f);
	}

	//Incrementing quantities of all products by 1
	for (std::vector<std::string>::const_iterator it = _reaction.GetProducts().begin(); it != _reaction.GetProducts().end(); it++)
	{
		dataState.GetSpecies(*it).Increment(1.f);
	}
}

void ECellEngine::Solvers::GillespieNRMRSolver::BuildDependancyGraph(const std::vector<std::string>& _reactions)
{
	std::vector<std::string> species;
	for (std::vector<std::string>::const_iterator itReactions = _reactions.begin(); itReactions != _reactions.end(); itReactions++)
	{
		species = dataState.GetReaction(*itReactions).GetProducts();
		for (std::vector<std::string>::iterator itSpecies = species.begin(); itSpecies != species.end(); itSpecies++)
		{
			auto reactionLinks = dataState.GetOperandsToOperations().equal_range(*itSpecies);
			for (auto itDependantReaction = reactionLinks.first; itDependantReaction != reactionLinks.second; itDependantReaction++)
			{
				reactionsDependanceGraph.emplace(*itReactions, (*itDependantReaction).second);
			}
		}
		/*
		species = dataState.GetReaction(*itReactions).GetReactants();
		for (std::vector<std::string>::iterator itSpecies = species.begin(); itSpecies != species.end(); itSpecies++)
		{
			auto reactionLinks = dataState.GetOperandsToOperations().equal_range(*itSpecies);
			for (auto itDependantReaction = reactionLinks.first; itDependantReaction != reactionLinks.second; itDependantReaction++)
			{
				reactionsDependanceGraph.emplace(*itReactions, *itDependantReaction);
			}
		}*/
	}
}

const float ECellEngine::Solvers::GillespieNRMRSolver::ComputeReactionPropensity(std::string& _reactionName)
{
	return dataState.GetReaction(_reactionName).ComputeKineticLaw();
}

void ECellEngine::Solvers::GillespieNRMRSolver::SolveBackward(const float& targetTime)
{

}

void ECellEngine::Solvers::GillespieNRMRSolver::SolveForward(const float& targetTime)
{

}

void ECellEngine::Solvers::GillespieNRMRSolver::Initialize(const ECellEngine::Data::Module& _module)
{
	std::vector<std::string> reactions = dynamic_cast<const ECellEngine::Data::BiochemicalModule&>(_module).GetAllReaction();

	//Build the data structure representing the dependency graph.
	BuildDependancyGraph(reactions);

	//Initializes the reversible Random Number Generator
	rng.Initialize(123);

	//Draw the first reaction times (named tau in the original algorithm)
	std::vector<std::pair<std::size_t, float>> taus;
	size_t idx = 0;
	for (std::vector<std::string>::iterator it = reactions.begin(); it != reactions.end(); it++)
	{
		taus.push_back(std::pair<std::size_t, float>(idx, rng.Exponential(dataState.GetReaction(*it).ComputeKineticLaw())));
		idx++;
	}

	//Builds the reaction times indexed priority queue (indexed Minheap)
	tauIMH.Initialize(taus);

	trace.reserve(1024);
}

void ECellEngine::Solvers::GillespieNRMRSolver::Update(const ECellEngine::Data::Module& _module, const float& _deltaTime)
{

}
