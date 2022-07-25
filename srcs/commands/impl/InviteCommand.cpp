#include "commands/impl/InviteCommand.hpp"
#include "Irc.hpp"

void InviteCommand::execute(const Command& cmd, Client& sender) {
	std::vector<std::string> args = cmd.getArgs();

	if (args.size() < 2) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "INVITE");
		return;
	}

	std::string targetNick = args[0];
	std::string targetChannelName = args[1];

	// Check if target nick exists
	if (Irc::getInstance().getServer()->nickIsUsed(targetNick) == false) {
		Irc::getInstance().getServer()->send_err_nosuchnick(sender, targetNick);
		return;
	}
	
	// Check if target channel exists
	if (Irc::getInstance().getServer()->getChannels().find(targetChannelName) == Irc::getInstance().getServer()->getChannels().end()) {
		Irc::getInstance().getServer()->send_err_nosuchchannel(sender, targetChannelName);
		return;
	}

	Client& targetClient = Irc::getInstance().getServer()->findClientByName(targetNick);
	Channel *targetChannel = &Irc::getInstance().getServer()->getChannels().find(targetChannelName)->second;

	// Check if sender is in the target channel
	if (!sender.isInChannel(*targetChannel)) {
		Irc::getInstance().getServer()->send_err_notonchannel(sender, targetChannelName);
		return;
	}

	// Check if target is already in the target channel
	if (targetClient.isInChannel(*targetChannel)) {
		Irc::getInstance().getServer()->send_err_useronchannel(sender, targetNick, targetChannelName);
		return;
	}

	// if channel has invite-only mode enabled, the sender needs to be channel operator to invite someone
	if (targetChannel->getClientsAndMode().at(sender.getSocket()) != CHANNEL_OP && targetChannel->hasMode(INVITE_ONLY)) {
		Irc::getInstance().getServer()->send_err_chanoprivsneeded(sender, targetChannelName);
		return;
    }

	targetChannel->addInvite(targetClient);
	sender.sendMessage(targetClient,  "INVITE " + targetNick + " :" + targetChannelName);
}