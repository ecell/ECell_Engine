#pragma once

/*
@file Command.hpp
@brief The base command class.
*/

#include <string>
#include <vector>

namespace ECellEngine::IO
{
	/*
	@brief		The base command class.
	@details	Adapted from the Command design pattern.
	*/
	class AbstractCommand
	{
	private:

		/*!
		@brief The name of the command.
		@details The name of the command is the key used to identify the command
				 in the command map of the commands manager.
		@see ECellEngine::IO::CommandsManager::commands
		*/
		const std::string name;


	protected:
		AbstractCommand(const char* _commandName, const unsigned char _nbArgs) :
			name(_commandName), nbArgs(_nbArgs)
		{
		};

	public:
		
		/*!
		@brief The number of arguments of the command including the command name.
		*/
		const unsigned char nbArgs;

		virtual ~AbstractCommand() = default;

		/*!
		@brief Gets the help message of the command.
		@return The const string of the help message.
		*/
		virtual const char* GetHelpMessage() const = 0;
		
		/*!
		@brief Returns the name of the command.
		*/
		inline std::string const& GetName() const noexcept
		{
			return name;
		}

		/*!
		@brief Public interface to decode the arguments of the command encoded
			   as a vector of strings.
		@param _args The arguments of the command. By default _args[0] is the
					 name of the command. So the actual arguments are store from
					 index 1 onward.
		*/
		virtual bool DecodeParameters(const std::vector<std::string>& _args) = 0;

		/*!
		@brief Public interface to execute the command disregarding any arguments.
		@details This method is meant to be called after DecodeParameters() has
				 been called. We make it public to allow the user to call it
				 directly in case the command doesn't require any arguments;
				 or the arguments hav
		@return True if the execution succeeds. False, otherwise.
		*/
		virtual bool Execute() = 0;

		/*
		@brief Decodes the command and executes it.
		@param _args The arguments of the command. By default _args[0] is the
					 name of the command. So the actual arguments are store from index
					 1 onward.
		@return True if the decoding and execution succeeds. False, otherwise.
		*/
		bool ProcessCommand(const std::vector<std::string>& _args);

	};

	/*!
	@brief Intermediate class to translate a command written as a string into
		   a function call.
	@details This class is meant to be inherited by a class that will implement
			 the an Execute() method. This method will be called when the command
			 is executed with a string input.
	*/
	template<typename ArgsType>
	struct Command : public AbstractCommand
	{
		/*!
		@brief The data structure that constains the arguments of the command.
		@details It is left to the user to define the structure of the arguments.
				 It can be declared as a member struct.
		*/
		ArgsType args;

		Command(const char* _name, unsigned char _nbArgs) :
			AbstractCommand(_name, _nbArgs)
		{

		}

		virtual const char* GetHelpMessage() const override = 0;
		
		/*!
		@brief Sets the arguments values for this command in ::args.
		*/
		inline void SetArgs(ArgsType& _args)
		{
			args = _args;
		}

		virtual bool DecodeParameters(const std::vector<std::string>& _args) override = 0;

		virtual bool Execute() override = 0;
	};
}
