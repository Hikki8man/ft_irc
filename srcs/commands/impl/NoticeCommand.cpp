#include "commands/impl/NoticeCommand.hpp"
#include "Irc.hpp"

void NoticeCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();

	if (args.empty() || args.size() == 1) { return; }

	std::vector<std::string> targets = split(args[0], ",");

	for (std::vector<std::string>::iterator targetIt = targets.begin(); targetIt != targets.end(); ++targetIt) {
		// if the target is a channel
		if (*targetIt->begin() == '#' || *targetIt->begin() == '&') {
			if (Irc::getInstance().getServer()->getChannels().find(*targetIt) == Irc::getInstance().getServer()->getChannels().end()) {
				continue;
			}
			Channel& channel = Irc::getInstance().getServer()->getChannels().find(*targetIt)->second;
			if (channel.findClientByName(sender.getNickname()).getNickname() != sender.getNickname()) {
				continue;
			}
			for (std::map<int, Channel::ClientAndMod>::const_iterator clientIt = channel.getClients().begin(); clientIt != channel.getClients().end(); ++clientIt) {
				if (clientIt->second.client.getNickname() != sender.getNickname()) {
					Irc::getInstance().getServer()->send_notice(sender.getPrefix(), clientIt->second.client, *targetIt, args[1]);
				}
			}
		}
		// if the target is a user
		else {
			if (Irc::getInstance().getServer()->nickIsUsed(*targetIt) == false) {
				Irc::getInstance().getServer()->send_err_nosuchnick(sender, *targetIt);
			}
			else // try notice the user
				Irc::getInstance().getServer()->send_notice(sender.getPrefix(), Irc::getInstance().getServer()->findClientByName(*targetIt), *targetIt, args[1]);
		}
	}
}