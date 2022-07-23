#include "commands/impl/WhoisCommand.hpp"
#include "Irc.hpp"

void WhoisCommand::execute(const Command& cmd, Client& sender) {
	std::vector<std::string> args = cmd.getArgs();

	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_nonicknamegiven(sender);
		return;
	}

	std::string target = args[0];
	if (Irc::getInstance().getServer()->nickIsUsed(target) == false) {
		Irc::getInstance().getServer()->send_err_nosuchnick(sender, target);
		Irc::getInstance().getServer()->send_rpl_endofwhois(sender, target);
		return;
	}

	Client& client = Irc::getInstance().getServer()->findClientByName(target);

	Irc::getInstance().getServer()->send_rpl_whoisuser(sender, client);
	Irc::getInstance().getServer()->send_rpl_whoishost(sender, client);
	Irc::getInstance().getServer()->send_rpl_endofwhois(sender, target);
}