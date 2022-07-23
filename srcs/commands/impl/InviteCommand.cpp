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

	// if channel has invite-only mode enabled, the sender needs to be channel operator to invite someone
	if (targetChannel->getClientsAndMod()[sender.getSocket()] != '@' && targetChannel->hasMode(INVITE_ONLY)) {
		Irc::getInstance().getServer()->send_err_chanoprivsneeded(sender, targetChannelName);
		return;
    }

	targetChannel->addInvite(targetClient);
	// TODO hostname
	targetClient.sendMessage(targetClient, sender.getNickname() + "!~" + sender.getUsername() + "@localhost INVITE " + targetNick + " :" + targetChannelName, false);
}