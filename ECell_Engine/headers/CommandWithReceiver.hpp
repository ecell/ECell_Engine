#pragma once

#include "Command.hpp"

/*
@brief Extension of the command pattern with a receiver.
*/
template <typename ReceiverType>
class CommandWithReceiver : public Command
{
protected:
	ReceiverType* receiver;

	CommandWithReceiver(char const* _commandName, ReceiverType* _receiver) :
		Command(_commandName), receiver(_receiver)
	{
	}

public:
	virtual void execute(const std::vector<std::string>& _args) override = 0;
};