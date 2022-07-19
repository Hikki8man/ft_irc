#ifndef IRC_COMMAND_EXECUTOR_HPP
#define IRC_COMMAND_EXECUTOR_HPP

#include "../Client.hpp"
#include "../common.hpp"
#include "Command.hpp"

class Command;

class CommandExecutor
{
	private:
		bool	_registered_only;
	public:
		virtual void execute(const Command& cmd, Client& sender) = 0;

		virtual ~CommandExecutor() {}

		CommandExecutor() : _registered_only(true) {};

		bool isRegisteredOnly() const { return _registered_only; }
		void setRegisteredOnly(bool registered_only) { _registered_only = registered_only; }
};


#endif