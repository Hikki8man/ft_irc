#include "commands/impl/QuitCommand.hpp"
#include "Irc.hpp"

QuitCommand::QuitCommand()
{
	this->setRegisteredOnly(false);
}

void QuitCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	std::string reason = "Client Quit";
	if (!args.empty())
		reason = args[0];
	std::map<std::string, Client> clientToSendQuit;
	for (std::map<std::string, const Channel &>::iterator it = sender.getChannels().begin(); it != sender.getChannels().end(); ++it) {
		std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(it->first);
		for (std::map<int, Channel::ClientAndMod>::const_iterator clientIt = channel->second.getClients().begin(); clientIt != channel->second.getClients().end(); ++clientIt) {
			if (clientIt->second.client.getNickname() != sender.getNickname()) {
				clientToSendQuit.insert(std::make_pair(clientIt->second.client.getNickname(), clientIt->second.client));
			}
		}
		channel->second.removeQuitClient(sender);
		if (channel->second.getClients().empty())
			Irc::getInstance().getServer()->getChannels().erase(channel);
	}
	for (std::map<std::string, Client>::iterator it = clientToSendQuit.begin(); it != clientToSendQuit.end(); ++it) {
		Irc::getInstance().getServer()->send_quit(sender, it->second, reason);
	}
	sender.getChannels().clear();
	Irc::getInstance().getServer()->getClients().erase(sender.getSocket());
	Irc::getInstance().getServer()->send_error(sender, reason);
}