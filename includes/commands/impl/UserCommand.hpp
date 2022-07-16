#ifndef IRC_USER_COMMAND_HPP
#define IRC_USER_COMMAND_HPP

#include "../CommandExecutor.hpp"

class UserCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, std::vector<std::string>& args, Client& sender);
};

#endif