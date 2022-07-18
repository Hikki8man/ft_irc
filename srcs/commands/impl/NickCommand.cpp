#include "commands/impl/NickCommand.hpp"
#include "Irc.hpp"

void NickCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_nonicknamegiven(sender);
		return;
	}
	// verify of characters are valid
	std::string nick(args[0]);
	if (nick.find_first_of(" ,*!?@.") != std::string::npos || nick.find_first_of("$:#&", 0, 1) != std::string::npos) {
		Irc::getInstance().getServer()->send_err_erroneusnickname(sender, nick);
		return;
	}
	if (nick.length() > NICKLEN)
		nick = nick.substr(0, NICKLEN);
	if (Irc::getInstance().getServer()->nickIsUsed(nick)) {
		Irc::getInstance().getServer()->send_err_nicknameinuse(sender, nick);
	} else {
		sender.setNickname(nick);
		sender.setPrefix();
	}
	
}