#include "commands/impl/PrivmsgCommand.hpp"
#include "Irc.hpp"

void PrivmsgCommand::execute(const Command& cmd, Client& sender) {
	// privmsg
	// :lithium.libera.chat 411 chak :No recipient given (PRIVMSG)
	// privmsg jchevet
	// lithium.libera.chat 412 chak :No text to send
	const std::vector<std::string>& args = cmd.getArgs();

	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_norecipient(sender, "PRIVMSG"); return;
	}

	std::vector<std::string> targets = split(args[0], ",");

	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_notexttosend(sender, "PRIVMSG"); return;
	}

	for (std::vector<std::string>::iterator targetIt = targets.begin(); targetIt != targets.end(); ++targetIt) {
		if (*targetIt->begin() == '#' || *targetIt->begin() == '&') {
			if (Irc::getInstance().getServer()->getChannels().find(*targetIt) == Irc::getInstance().getServer()->getChannels().end()) {
				Irc::getInstance().getServer()->send_err_nosuchchannel(sender, *targetIt);
				continue;
			}
			Channel& channel = Irc::getInstance().getServer()->getChannels().find(*targetIt)->second;
			if (channel.getClient(sender.getNickname()).getNickname() != sender.getNickname()) {
				Irc::getInstance().getServer()->send_err_cannotsendtochan(sender, *targetIt);
				continue;
			}
			for (std::map<int, Channel::ClientAndMod>::const_iterator clientIt = channel.getClients().begin(); clientIt != channel.getClients().end(); ++clientIt) {
				if (clientIt->second.client.getNickname() != sender.getNickname()) {
					Irc::getInstance().getServer()->send_privmsg(sender, clientIt->second.client, *targetIt, args[1]);
				}
			}
		}
		else {
			if (Irc::getInstance().getServer()->nickIsUsed(*targetIt) == false) {
				Irc::getInstance().getServer()->send_err_nosuchnick(sender, *targetIt);
			}
			else
				Irc::getInstance().getServer()->send_privmsg(sender, Irc::getInstance().getServer()->getClient(*targetIt), *targetIt, args[1]);
		}
	}
}