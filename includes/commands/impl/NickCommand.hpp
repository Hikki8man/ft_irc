#ifndef IRC_NICK_COMMAND_HPP
#define IRC_NICK_COMMAND_HPP

#include "../CommandExecutor.hpp"

class NickCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, std::vector<std::string>& args, Client& sender);
};

#endif