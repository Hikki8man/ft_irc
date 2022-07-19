#ifndef IRC_PASS_COMMAND_HPP
#define IRC_PASS_COMMAND_HPP

#include "../CommandExecutor.hpp"

class PassCommand : public CommandExecutor
{
	public:
		PassCommand();
		void execute(const Command& cmd, Client& sender);
};

#endif