#include "commands/impl/NickCommand.hpp"
#include "Irc.hpp"

void NickCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	// verify of characters are valid
	std::string restrict(" ,*!@.$:#&");
	if (args[0].find_first_of(restrict) != std::string::npos) {
		//send proper error message
		std::cout << "NICK: invalid characters" << std::endl;
		return;
	}
	// ---
	// verify if nick is already used
	if (Irc::getInstance().getServer()->nickIsUsed(args[1])) {
		// send error message
		std::cout << "Nickname already used" << std::endl;
	} else
		sender.setNickname(args[0]);
}