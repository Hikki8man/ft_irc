#ifndef IRC_MOTD_COMMAND_HPP
#define IRC_MOTD_COMMAND_HPP

#include "../CommandExecutor.hpp"

class MotdCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif