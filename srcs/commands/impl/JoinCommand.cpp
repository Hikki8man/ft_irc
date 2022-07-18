#include "commands/impl/JoinCommand.hpp"

void JoinCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();

	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "JOIN");
		return; 
	}

	std::string channels = args[0];
	std::string keys;

	if (args.size() > 1) {
		keys = args[1];
	}
	std::vector<std::string> channelsList = split(channels, ",");
	std::vector<std::string> keysList = split(keys, ",");
	
	for (std::vector<std::string>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		if ((*it->begin() != '#' && *it->begin() != '&') || (it->find(' ') != std::string::npos || it->find(',') != std::string::npos || it->find('\a') != std::string::npos)) {
			Irc::getInstance().getServer()->send_err_nosuchchannel(sender, *it);
			continue;
		}
		std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(*it);
		if (channel == Irc::getInstance().getServer()->getChannels().end()) {
			// create channel
			Channel& newChannel = Irc::getInstance().getServer()->getChannels().insert(std::make_pair(*it, Channel(*it))).first->second;
			newChannel.addClient(sender);
			if (!keysList.empty()) {
				newChannel.setKey(keysList.front());
				keysList.erase(keysList.begin());
			}
		}
		else {
			// add client to channel
			if (!keysList.empty()) {
				channel->second.addClient(sender, keysList.front());
				keysList.erase(keysList.begin());
			}
			else {
				channel->second.addClient(sender);
			}
		}
	}	
}