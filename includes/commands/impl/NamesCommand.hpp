#ifndef IRC_NAMES_COMMAND_HPP
#define IRC_NAMES_COMMAND_HPP

#include "../CommandExecutor.hpp"

class NamesCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif