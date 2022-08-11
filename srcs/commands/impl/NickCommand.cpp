#include "commands/impl/NickCommand.hpp"
#include "Irc.hpp"

NickCommand::NickCommand()
{
	this->setRegisteredOnly(false);
}

void NickCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_nonicknamegiven(sender);
		return;
	}
	// verify of characters are valid
	std::string nick(args[0]);
	if (nick.find_first_of(" ,*!?@.") != std::string::npos || nick.substr(0, 1).find_first_of("$:#&") != std::string::npos) {
		Irc::getInstance().getServer()->send_err_erroneusnickname(sender, nick);
		return;
	}
	if (nick.length() > NICKLEN)
		nick = nick.substr(0, NICKLEN);
	if (Irc::getInstance().getServer()->nickIsUsed(nick)) {
		Irc::getInstance().getServer()->send_err_nicknameinuse(sender, nick);
	} else {
		if (sender.getNickname().empty())
			sender.setNickname(nick);
		else {
			sender.setNickname(nick);
			for (std::map<std::string, const Channel &>::iterator it = sender.getChannels().begin(); it != sender.getChannels().end(); ++it) {
				std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(it->first);
				for (std::map<int, char>::const_iterator clientIt = channel->second.getClientsAndMode().begin(); clientIt != channel->second.getClientsAndMode().end(); ++clientIt) {
					if (clientIt->first != sender.getSocket())
						sender.sendMessage(clientIt->first, "NICK :" + nick);
				}
			}
			sender.sendMessage(sender.getSocket(), "NICK :" + nick);
		}
		sender.setPrefix();
	}
}