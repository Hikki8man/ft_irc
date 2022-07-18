#include "commands/impl/PartCommand.hpp"
#include "Irc.hpp"

void PartCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "PART");
		return;
	}
	std::vector<std::string> channelsList = split(args[0], ",");
	std::string reason;

	if (args.size() > 1)
		reason = args[1];
	
	for (std::vector<std::string>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(*it);
		if (channel == Irc::getInstance().getServer()->getChannels().end()) {
			Irc::getInstance().getServer()->send_err_nosuchchannel(sender, *it);
			continue;
		}
		else if (channel->second.getClients().find(sender.getSocket()) == channel->second.getClients().end()) {
			Irc::getInstance().getServer()->send_err_notonchannel(sender, *it);
			continue;
		}
		else {
			channel->second.removeClient(sender, reason);
			if (channel->second.getClients().empty())
				Irc::getInstance().getServer()->getChannels().erase(channel);
		}
	}
}