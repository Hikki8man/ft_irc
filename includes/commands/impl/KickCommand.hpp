#ifndef IRC_KICK_COMMAND_HPP
#define IRC_KICK_COMMAND_HPP

#include "../CommandExecutor.hpp"

class KickCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif