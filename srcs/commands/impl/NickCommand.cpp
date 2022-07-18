#include "commands/impl/NickCommand.hpp"
#include "Irc.hpp"

void NickCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		Irc::getInstance().getServer()->send_err_nonicknamegiven(sender);
		return;
	}
	// verify of characters are valid
	std::string restrict(" ,*!@.$:#&");
	if (args[0].find_first_of(restrict) != std::string::npos) {
		Irc::getInstance().getServer()->send_err_erroneusnickname(sender, args[0]);
		return;
	}
	// verify if nick is already used
	if (Irc::getInstance().getServer()->nickIsUsed(args[1])) {
		Irc::getInstance().getServer()->send_err_nicknameinuse(sender, args[0]);
		return;
	} else
		sender.setNickname(args[0]);
	
	sender.setPrefix();
}