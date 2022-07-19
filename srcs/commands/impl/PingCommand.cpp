#include "commands/impl/PingCommand.hpp"
#include "Irc.hpp"

void PingCommand::execute(const Command& cmd, Client& sender) {
	if (cmd.getArgs().size() < 1) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "PING");
	} else {
		std::string msg = Irc::getInstance().getServer()->getPrefix() + " PONG " + Irc::getInstance().getServer()->getIp() + " :" + cmd.getArgs()[0] + CRLF;
		send(sender.getSocket(), msg.c_str(), msg.length(), 0);
	}
}