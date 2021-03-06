#ifndef IRC_NICK_COMMAND_HPP
#define IRC_NICK_COMMAND_HPP

#include "../CommandExecutor.hpp"

class NickCommand : public CommandExecutor
{
	public:
		NickCommand();
		void execute(const Command& cmd, Client& sender);
};

#endif