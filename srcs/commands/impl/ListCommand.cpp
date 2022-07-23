#include "commands/impl/ListCommand.hpp"
#include "Irc.hpp"

void ListCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
		Irc::getInstance().getServer()->send_rpl_liststart(sender);
	if (args.empty()) {
		for (std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().begin(); channel != Irc::getInstance().getServer()->getChannels().end(); ++channel) {
			if (!channel->second.hasMode(SECRET)) {
				Irc::getInstance().getServer()->send_rpl_list(sender, channel->second);
			}
		}
	}
	else {
		std::vector<std::string> channelsList = split(args[0], ",");
		for (std::vector<std::string>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
			std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(*it);
			if (channel == Irc::getInstance().getServer()->getChannels().end() || channel->second.hasMode(SECRET)) {
				continue;
			} else {
				Irc::getInstance().getServer()->send_rpl_list(sender, channel->second);
			}
		}
	}
	Irc::getInstance().getServer()->send_rpl_listend(sender);
}