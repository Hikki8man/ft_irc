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
	std::map<SOCKET, int> clientToSendQuit;
	for (std::map<std::string, const Channel &>::iterator it = sender.getChannels().begin(); it != sender.getChannels().end(); ++it) {
		std::map<std::string, Channel>::iterator channel = Irc::getInstance().getServer()->getChannels().find(it->first);
		for (std::map<int, char>::const_iterator clientIt = channel->second.getClientsAndMod().begin(); clientIt != channel->second.getClientsAndMod().end(); ++clientIt) {
			if (clientIt->first != sender.getSocket()) {
				clientToSendQuit[clientIt->first];
			}
		}
		channel->second.removeQuitClient(sender);
		if (channel->second.getClientsAndMod().empty())
			Irc::getInstance().getServer()->getChannels().erase(channel);
	}
	for (std::map<SOCKET, int>::iterator it = clientToSendQuit.begin(); it != clientToSendQuit.end(); ++it) {
		std::string msg = "QUIT :" + reason;
		sender.sendMessage(it->first, msg);
	}
	std::string msg_to_send = "ERROR :Closing Link: " + sender.getIp() + " (" + reason + ")";
	sender.sendMessage(sender, msg_to_send, false);
	Irc::getInstance().getServer()->getClients().erase(sender.getSocket());
	close(sender.getSocket());
}