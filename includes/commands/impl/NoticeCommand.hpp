#ifndef IRC_NOTICE_COMMAND_HPP
#define IRC_NOTICE_COMMAND_HPP

#include "../CommandExecutor.hpp"
// #include "../../Server.hpp"

// class Server;
// class Command;

class NoticeCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
		// void execute(const Command& cmd, Server& sender);
};

#endif
