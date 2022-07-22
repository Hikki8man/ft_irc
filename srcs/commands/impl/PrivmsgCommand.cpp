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
			// if client is not in the channel ans channel has no external messages mode enabled, send error
			if (channel.findClientByName(sender.getNickname()).getNickname() != sender.getNickname() && channel.hasMode(NO_EXTERNAL_MESSAGES)) {
				Irc::getInstance().getServer()->send_err_cannotsendtochan(sender, *targetIt);
				continue;
			}
			for (std::map<int, Channel::ClientAndMod>::const_iterator clientIt = channel.getClients().begin(); clientIt != channel.getClients().end(); ++clientIt) {
				if (clientIt->second.client.getNickname() != sender.getNickname()) {
					Irc::getInstance().getServer()->send_privmsg(sender, clientIt->second.client, *targetIt, args[1]);
				}
			}
		}
		// if the target is a user
		else {
			if (Irc::getInstance().getServer()->nickIsUsed(*targetIt) == false) {
				Irc::getInstance().getServer()->send_err_nosuchnick(sender, *targetIt);
			}
			else // try privmsg the user
				Irc::getInstance().getServer()->send_privmsg(sender, Irc::getInstance().getServer()->findClientByName(*targetIt), *targetIt, args[1]);
		}
	}
}