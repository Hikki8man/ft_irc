#include "commands/impl/NamesCommand.hpp"
#include "Irc.hpp"


void NamesCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	
	if (args.empty())
	{
		for (std::map<std::string, Channel>::iterator channelIt = Irc::getInstance().getServer()->getChannels().begin(); channelIt != Irc::getInstance().getServer()->getChannels().end(); ++channelIt)
			Irc::getInstance().getServer()->send_rpl_namreply(sender, channelIt->second);
		Channel privChannel("*");
		for (std::map<int, Client>::iterator clientIt = Irc::getInstance().getServer()->getClients().begin(); clientIt != Irc::getInstance().getServer()->getClients().end(); ++clientIt) {
			if (clientIt->second.getChannels().empty()) {
				privChannel.getClientsAndMod().insert(std::pair<int, char>(clientIt->first, ' '));
			}
		}
		if (!privChannel.getClientsAndMod().empty())
			Irc::getInstance().getServer()->send_rpl_namreply(sender, privChannel);
		Irc::getInstance().getServer()->send_rpl_endofnames(sender, privChannel);
	}
	else {
		std::vector<std::string> channelList = split(args[0], ",");
		std::vector<std::string>::iterator channelIt = channelList.begin();
		for (; channelIt != channelList.end(); ++channelIt)
		{
			std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(*channelIt);
			if (channel != Irc::getInstance().getServer()->getChannels().end())
				Irc::getInstance().getServer()->send_rpl_namreply(sender, channel->second);
		}
		Irc::getInstance().getServer()->send_rpl_endofnames(sender, *(--channelIt));
	}
}