#include "commands/impl/MotdCommand.hpp"
#include "Irc.hpp"

void MotdCommand::execute(const Command& cmd, Client& sender) {
	static_cast<void>(cmd);
	Irc::getInstance().getServer()->send_rpl_motdstart(sender);
	Irc::getInstance().getServer()->send_rpl_motd(sender, "Ouais ouais ouais le MOTD");
	Irc::getInstance().getServer()->send_rpl_endofmotd(sender);
}