#pragma once

#include <string>
#include <vector>

namespace ECellEngine::IO
{
	/*
	@brief		The base command class.
	@details	Adapted from the Command design pattern.
	*/
	class Command
	{
	private:
		const std::string name;

	protected:
		Command(const char* _commandName) :
			name(_commandName)
		{
		};

	public:
		Command(Command&&) = default;

		Command(Command const&) = default;

		virtual ~Command() = default;

		inline std::string const& GetName() const noexcept
		{
			return name;
		}

		/*
		@brief Public interface to execute the action/code associated to the command
				and its arguments.
		@param[in] _args The arguments of the command. By default _args[0] is the
					name of the command. So the actual arguments are store from index
					1 onward.
		@return True if the execution succeeds. False, otherwise.
		*/
		virtual bool Execute(const std::vector<std::string>& _args) = 0;
	};
}
