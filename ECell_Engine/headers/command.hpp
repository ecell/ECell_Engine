#pragma once

class Command
{
protected:
	Command(){};

public:
	virtual ~Command(){};

	/// <summary>
	/// Public interface to execute the action/code
	/// associated to the command.
	/// </summary>
	virtual void Execute() = 0;
};