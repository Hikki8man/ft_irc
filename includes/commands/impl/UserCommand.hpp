#ifndef IRC_USER_COMMAND_HPP
#define IRC_USER_COMMAND_HPP

#include "../CommandExecutor.hpp"

class UserCommand : public CommandExecutor
{
	public:
		UserCommand();
		void execute(const Command& cmd, Client& sender);
};

#endif