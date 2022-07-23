#ifndef IRC_WHOIS_COMMAND_HPP
#define IRC_WHOIS_COMMAND_HPP

#include "../CommandExecutor.hpp"

class WhoisCommand : public CommandExecutor
{
	public:
		void execute(const Command& cmd, Client& sender);
};

#endif