#ifndef IRC_TIME_COMMAND_HPP
#define IRC_TIME_COMMAND_HPP

#include "../CommandExecutor.hpp"

class TimeCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif