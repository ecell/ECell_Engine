#include "Util/BinarySearch.hpp"
#include "Data/DataState.hpp"

Operand* ECellEngine::Data::DataState::GetOperand(const std::size_t _id)
{
	std::list<std::pair<const std::size_t, std::shared_ptr<Parameter>>>::iterator s1 = parameters.find(_id);
	if (s1 != parameters.end())
	{
		return s1->second.get();
	}

	std::list<std::pair<const std::size_t, std::shared_ptr<Species>>>::iterator s3 = species.find(_id);
	if (s3 != species.end())
	{
		return s3->second.get();
	}

	return s3->second.get();//this is null
}

void ECellEngine::Data::DataState::Clear() noexcept
{
	species.clear();
	parameters.clear();
	equations.clear();
	reactions.clear();

	operandsToOperations.clear();

	operations.clear();
	logicOperations.clear();
	modifyDataStateValueEvents.clear();
	triggers.clear();
}

bool ECellEngine::Data::DataState::EraseAllDataOfType(const char* _dataType) noexcept
{
	if (!strcmp(_dataType, "Equation"))
	{
		equations.clear();
	}
    else if (!strcmp(_dataType, "Parameter"))
	{
		parameters.clear();
	}
	else if (!strcmp(_dataType, "Reaction"))
	{
		reactions.clear();
	}
	else if (!strcmp(_dataType, "Species"))
	{
		species.clear();
	}
	else if (!strcmp(_dataType, "OperandsToOperation"))
	{
		operandsToOperations.clear();
	}
	else if (!strcmp(_dataType, "Arithmetic"))
	{
		operations.clear();
	}
	else if (!strcmp(_dataType, "Logic"))
	{
		logicOperations.clear();
	}
	else if (!strcmp(_dataType, "ModifyDataStateValueEvent"))
	{
		modifyDataStateValueEvents.clear();
	}
	else if (!strcmp(_dataType, "Trigger"))
	{
		triggers.clear();
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("Invalid data of type \"%s\": there is no container to be matched with this data type in the datastate.", _dataType);
		return false;
	}
	return true;
}

bool ECellEngine::Data::DataState::EraseDataOfType(const char* _dataType, const std::size_t _id) noexcept
{
	if (!strcmp(_dataType, "Equation"))
	{
		if (!equations.erase(_id))
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"Equation\" with ID %llu.", _id);
			return false;
		}
	}
    else if (!strcmp(_dataType, "Parameter"))
	{
		if (!parameters.erase(_id))
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"Parameter\" with ID %llu.", _id);
			return false;
		}
	}
	else if (!strcmp(_dataType, "Reaction"))
	{
		if (!reactions.erase(_id))
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"Reaction\" with ID %llu.", _id);
			return false;
		}
	}
	else if (!strcmp(_dataType, "Species"))
	{
		if (!species.erase(_id))
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"Species\" with ID %llu.", _id);
			return false;
		}
	}
	else if (!strcmp(_dataType, "OperandsToOperation"))
	{
		if (!operandsToOperations.erase(_id))
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"OperandsToOperation\" with ID %llu.", _id);
			return false;
		}
	}
	else if (!strcmp(_dataType, "Arithmetic"))
	{
		std::vector<std::shared_ptr<Maths::Operation>>::iterator opIt = Util::BinarySearch::LowerBound(operations.begin(), operations.end(), _id, [](const std::shared_ptr<Maths::Operation>& _op, const std::size_t _id) { return _op->GetID() < _id; });
		if (opIt == operations.end() || (*opIt)->GetID() != _id)
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"Arithmetic\" with ID %llu.", _id);
			return false;
		}
		operations.erase(opIt);
	}
	else if (!strcmp(_dataType, "Logic"))
	{
		std::vector<std::shared_ptr<Maths::LogicOperation>>::iterator opIt = Util::BinarySearch::LowerBound(logicOperations.begin(), logicOperations.end(), _id, [](const std::shared_ptr<Maths::LogicOperation>& _op, const std::size_t _id) { return _op->GetID() < _id; });
		if (opIt == logicOperations.end() || (*opIt)->GetID() != _id)
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"Logic\" with ID %llu.", _id);
			return false;
		}
		logicOperations.erase(opIt);
	}
	else if (!strcmp(_dataType, "ModifyDataStateValueEvent"))
	{
		std::vector<std::shared_ptr<Core::Events::ModifyDataStateValueEvent>>::iterator evIt = Util::BinarySearch::LowerBound(modifyDataStateValueEvents.begin(), modifyDataStateValueEvents.end(), _id, [](const std::shared_ptr<Core::Events::ModifyDataStateValueEvent>& _ev, const std::size_t _id) { return _ev->GetID() < _id; });
		if (evIt == modifyDataStateValueEvents.end() || (*evIt)->GetID() != _id)
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"ModifyDataStateValueEvent\" with ID %llu.", _id);
			return false;
		}
		modifyDataStateValueEvents.erase(evIt);
	}
	else if (!strcmp(_dataType, "Trigger"))
	{
		std::vector<std::shared_ptr<Core::Trigger<Operand*, Operand*>>>::iterator tgrIt = Util::BinarySearch::LowerBound(triggers.begin(), triggers.end(), _id, [](const std::shared_ptr<Core::Trigger<Operand*, Operand*>>& _tgr, const std::size_t _id) { return _tgr->GetID() < _id; });
		if (tgrIt == triggers.end() || (*tgrIt)->GetID() != _id)
		{
			ECellEngine::Logging::Logger::LogError("EraseDataOfType: Could not erase data of type \"Trigger\" with ID %llu.", _id);
			return false;
		}
		triggers.erase(tgrIt);
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("Invalid data of type \"%s\": there is no container to be matched with this data type in the datastate.", _dataType);
		return false;
	}
	return true;
}

void ECellEngine::Data::DataState::Reset() noexcept
{
	for (auto& [spName, _sp] : species)
	{
		_sp->Reset();
	}

	for (auto& [pName, _p] : parameters)
	{
		_p->Reset();
	}

	for (auto& [eqName, eq] : equations)
	{
		eq->Reset();
	}

	for (auto& [rName, _r] : reactions)
	{
		_r->ComputeKineticLaw();
	}
}
