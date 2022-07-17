#ifndef IRC_COMMAND_EXECUTOR_HPP
#define IRC_COMMAND_EXECUTOR_HPP

#include "../Client.hpp"
#include "../common.hpp"
#include "Command.hpp"

class Command;

class CommandExecutor
{
	public:
		virtual void execute(const Command& cmd, Client& sender) = 0;

		virtual ~CommandExecutor() {}
};


#endif