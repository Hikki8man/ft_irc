#ifndef IRC_TOPIC_COMMAND_HPP
#define IRC_TOPIC_COMMAND_HPP

#include "../CommandExecutor.hpp"

class TopicCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif