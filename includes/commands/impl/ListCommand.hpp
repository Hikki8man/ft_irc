#ifndef IRC_LIST_COMMAND_HPP
#define IRC_LIST_COMMAND_HPP

#include "../CommandExecutor.hpp"

class ListCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif