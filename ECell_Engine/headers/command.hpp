#pragma once


/*
@brief		The base command class.
@details	Adapted from the Command design pattern.
*/
class Command
{
private:
	const std::string name;

protected:
	Command(char const* _commandName):
		name(_commandName)
	{};

public:
	Command(Command&&) = default;

	Command(Command const&) = default;

	virtual ~Command() = default;

	inline std::string const& getName() const noexcept
	{
		return name;
	}

	/*
	@brief Public interface to execute the action/code
			associated to the command.
	*/
	virtual void execute() = 0;
};