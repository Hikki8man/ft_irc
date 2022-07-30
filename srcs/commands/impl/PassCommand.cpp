#include "commands/impl/PassCommand.hpp"
#include "Irc.hpp"

PassCommand::PassCommand() {
	this->setRegisteredOnly(false);
}

void PassCommand::execute(const Command& cmd, Client& sender) {
	std::vector<std::string> args = cmd.getArgs();

	if (args.size() == 0) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "PASS");
		// TODO close connection with ERROR
		return;
	}
	if (sender.isLogged()) {
		Irc::getInstance().getServer()->send_err_alreadyregistered(sender);
		return;
	}
	if (args[0] != Irc::getInstance().getServer()->getPassword()) {
		Irc::getInstance().getServer()->send_err_passwdmismatch(sender);
		return;
	}
	sender.setLogged(true);
}