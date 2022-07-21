#ifndef IRC_MODE_COMMAND_HPP
#define IRC_MODE_COMMAND_HPP

#include "../CommandExecutor.hpp"

class ModeCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif