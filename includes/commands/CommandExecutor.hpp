#ifndef IRC_COMMAND_EXECUTOR_HPP
#define IRC_COMMAND_EXECUTOR_HPP

#include "../Client.hpp"
#include "../common.hpp"
#include "Command.hpp"

class Command;

class CommandExecutor
{
	public:
		virtual void execute(const Command& cmd, std::vector<std::string>& args, Client& sender) = 0;

		virtual ~CommandExecutor() {}
};


#endif