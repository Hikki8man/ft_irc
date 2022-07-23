#include "commands/impl/PrivmsgCommand.hpp"
#include "Irc.hpp"

void PrivmsgCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();

	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_norecipient(sender, "PRIVMSG"); return;
	}

	std::vector<std::string> targets = split(args[0], ",");

	if (args.size() == 1) {
		Irc::getInstance().getServer()->send_err_notexttosend(sender, "PRIVMSG"); return;
	}

	for (std::vector<std::string>::iterator targetIt = targets.begin(); targetIt != targets.end(); ++targetIt) {
		// if the target is a channel
		if (*targetIt->begin() == '#' || *targetIt->begin() == '&') {
			if (Irc::getInstance().getServer()->getChannels().find(*targetIt) == Irc::getInstance().getServer()->getChannels().end()) {
				Irc::getInstance().getServer()->send_err_nosuchchannel(sender, *targetIt);
				continue;
			}
			Channel& channel = Irc::getInstance().getServer()->getChannels().find(*targetIt)->second;
			// if client is not in the channel and channel has no external messages mode enabled, send error
			if (!sender.isInChannel(channel) && channel.hasMode(NO_EXTERNAL_MESSAGES)) {
				Irc::getInstance().getServer()->send_err_cannotsendtochan(sender, *targetIt);
				continue;
			}
			for (std::map<int, char>::const_iterator clientIt = channel.getClientsAndMod().begin(); clientIt != channel.getClientsAndMod().end(); ++clientIt) {
				if (clientIt->first != sender.getSocket()) {
					std::string msg = "PRIVMSG " + *targetIt + " :" + args[1];
					sender.sendMessage(clientIt->first, msg);
				}
			}
		}
		// if the target is a user
		else {
			if (Irc::getInstance().getServer()->nickIsUsed(*targetIt) == false) {
				Irc::getInstance().getServer()->send_err_nosuchnick(sender, *targetIt);
			}
			else {// try privmsg the user
				std::string msg = "PRIVMSG " + *targetIt + " :" + args[1];
				sender.sendMessage(Irc::getInstance().getServer()->findClientByName(*targetIt).getSocket(), msg);
			}
		}
	}
}