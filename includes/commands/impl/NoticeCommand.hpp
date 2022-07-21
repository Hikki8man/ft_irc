#ifndef IRC_NOTICE_COMMAND_HPP
#define IRC_NOTICE_COMMAND_HPP

#include "../CommandExecutor.hpp"

class NoticeCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif
