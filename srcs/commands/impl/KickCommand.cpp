#include "commands/impl/KickCommand.hpp"
#include "Irc.hpp"

void KickCommand::execute(const Command& cmd, Client& sender) {
	std::vector<std::string> args = cmd.getArgs();

	if (args.size() < 2) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "KICK");
		return;
	}

	std::string targetChannelName = args[0];
	std::vector<std::string> targetNicks = split(args[1], ",");

	// Check if target channel exists
	if (Irc::getInstance().getServer()->getChannels().find(targetChannelName) == Irc::getInstance().getServer()->getChannels().end()) {
		Irc::getInstance().getServer()->send_err_nosuchchannel(sender, targetChannelName);
		return;
	}
	
	Channel *targetChannel = &Irc::getInstance().getServer()->getChannels().find(targetChannelName)->second;

	// Check if client is in the target channel
	if (!sender.isInChannel(*targetChannel)) {
		Irc::getInstance().getServer()->send_err_notonchannel(sender, targetChannelName);
		return;
	}

	// Check if sender is channel operator
	if (targetChannel->getClientsAndMode().at(sender.getSocket()) != CHANNEL_OP) {
		Irc::getInstance().getServer()->send_err_chanoprivsneeded(sender, targetChannelName);
		return;
	}

	for (std::vector<std::string>::iterator it = targetNicks.begin(); it != targetNicks.end(); ++it) {

		// Check if target nick exists
		if (Irc::getInstance().getServer()->nickIsUsed(*it) == false) {
			Irc::getInstance().getServer()->send_err_usernotinchannel(sender, *it, targetChannelName);
			return;
		}
		
		Client& targetClient = Irc::getInstance().getServer()->findClientByName(*it);

		// Check if target client is in the target channel
		if (!targetClient.isInChannel(*targetChannel)) {
			Irc::getInstance().getServer()->send_err_usernotinchannel(sender, *it, targetChannelName);
			return;
		}

		// Remove client from channel
		std::string reason = *it;
		if (args.size() > 2) {
			reason = args[2];
		}
		std::string msg = "KICK " + targetChannelName + " " + *it + " :" + reason;
		for (std::map<SOCKET, char>::const_iterator clientIt = targetChannel->getClientsAndMode().begin(); clientIt != targetChannel->getClientsAndMode().end(); ++clientIt)
			sender.sendMessage(clientIt->first, msg);
	
		targetChannel->removeClient(targetClient, cmd.getName(), reason);
		targetClient.removeChannel(*targetChannel);
	}
}