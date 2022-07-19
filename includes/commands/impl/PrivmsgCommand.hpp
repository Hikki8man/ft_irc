#ifndef IRC_PRIVMSG_COMMAND_HPP
#define IRC_PRIVMSG_COMMAND_HPP

#include "../CommandExecutor.hpp"

class PrivmsgCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif
