#pragma once

///<summary>
/// The base command class.
/// Extracted from the Command design pattern and used accordingly.
///</summary>
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