#include "commands/impl/TopicCommand.hpp"
#include "Irc.hpp"

void TopicCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	
	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "TOPIC");
		return;
	}

	std::vector<std::string> channelsList = split(args[0], ",");
	for (std::vector<std::string>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(*it);
		if (channel == Irc::getInstance().getServer()->getChannels().end()) {
			Irc::getInstance().getServer()->send_err_nosuchchannel(sender, *it);
			continue;
		} // If no topic is specified, display the topic of the channe
		else if (args.size() == 1) {
			if (channel->second.getTopic().empty())
				Irc::getInstance().getServer()->send_rpl_notopic(sender, channel->second);
			else {
				Irc::getInstance().getServer()->send_rpl_topic(sender, channel->second);
				Irc::getInstance().getServer()->send_rpl_topicwhotime(sender, channel->second);
			}
		}
		else {
			std::string topic = args[1];
			if (!sender.isInChannel(channel->second)) {
				Irc::getInstance().getServer()->send_err_notonchannel(sender, *it);
				continue;
			}
			else if (channel->second.hasMode(PROTECTED_TOPIC) && channel->second.getClientMode(sender.getSocket()) != CHANNEL_OP) {
				Irc::getInstance().getServer()->send_err_chanoprivsneeded(sender, *it);
				continue;
			}
			else {
				channel->second.setTopic(sender, topic);
			}
		}
	}
}