#ifndef IRC_QUIT_COMMAND_HPP
#define IRC_QUIT_COMMAND_HPP

#include "../CommandExecutor.hpp"

class QuitCommand : public CommandExecutor
{
	public:
		QuitCommand();
		void execute(const Command& cmd, Client& sender);
};

#endif
