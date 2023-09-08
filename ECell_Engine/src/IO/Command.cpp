#include "Logging/Logger.hpp"
#include "IO/Command.hpp"

bool ECellEngine::IO::AbstractCommand::ProcessCommand(const std::vector<std::string>& _args)
{
	if (!DecodeParameters(_args))
	{
		ECellEngine::Logging::Logger::LogError(GetHelpMessage());
		return false;
	}
	return Execute();
}