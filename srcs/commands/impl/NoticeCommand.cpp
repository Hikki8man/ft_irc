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
			if (!sender.isInChannel(channel) && channel.hasMode(NO_EXTERNAL_MESSAGES)) {
				continue;
			}
			for (std::map<int, char>::const_iterator clientIt = channel.getClientsAndMod().begin(); clientIt != channel.getClientsAndMod().end(); ++clientIt) {
				if (clientIt->first != sender.getSocket()) {
					std::string msg = "NOTICE " + channel.getName() + " :" + args[1];
					sender.sendMessage(clientIt->first, msg);
				}
			}
		}
		// if the target is a user
		else {
			if (Irc::getInstance().getServer()->nickIsUsed(*targetIt)) {
				std::string msg = "NOTICE " + *targetIt + " :" + args[1];
				sender.sendMessage(Irc::getInstance().getServer()->findClientByName(*targetIt), msg);
			}
		}
	}
}
