#ifndef IRC_JOIN_COMMAND_HPP
#define IRC_JOIN_COMMAND_HPP

#include "../CommandExecutor.hpp"

class JoinCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif