#include "commands/impl/UserCommand.hpp"
#include "Irc.hpp"

UserCommand::UserCommand() {
	this->setRegisteredOnly(false);
}

void UserCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();

	if (sender.isRegistered()) {
		Irc::getInstance().getServer()->send_err_alreadyregistered(sender);
	}
	else if (args.size() < 4) {
		Irc::getInstance().getServer()->send_err_needmoreparams(sender, "USER");
	}
	else if (Irc::getInstance().getServer()->userIsUsed(args[0])) {
		std::string nick = args[0];
		if (nick.empty())
			nick = "*";
		Irc::getInstance().getServer()->sendMessage(sender, "NOTICE " + nick + " :Username is already used.");
	}
	else {
		// send notice if username is invalid ?
		std::string user(args[0]);
		if (user.length() > USERLEN)
			user = user.substr(0, USERLEN);
		sender.setUsername(user);
		sender.setRealName(args[3]);
	}
}