#ifndef IRC_PING_COMMAND_HPP
#define IRC_PING_COMMAND_HPP

#include "../CommandExecutor.hpp"

class PingCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif