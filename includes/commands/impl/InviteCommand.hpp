#ifndef IRC_INVITE_COMMAND_HPP
#define IRC_INVITE_COMMAND_HPP

#include "../CommandExecutor.hpp"

class InviteCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif