#ifndef IRC_PART_COMMAND_HPP
#define IRC_PART_COMMAND_HPP

#include "../CommandExecutor.hpp"

class PartCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif