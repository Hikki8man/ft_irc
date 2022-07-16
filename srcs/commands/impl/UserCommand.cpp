#include "commands/impl/UserCommand.hpp"
#include "Irc.hpp"

void UserCommand::execute(const Command& cmd, std::vector<std::string>& args, Client& sender)
{
	// verify of characters are valid
	// ---
	// verify if User already registered
	if (sender.isRegistered()) {
		// send error message
		std::cout << "User already registered" << std::endl;
	}
	// verify if User is already used
	else if (Irc::getInstance().getServer().userIsUsed(args[1])) {
		// send error message
		std::cout << "Username already used" << std::endl;
	} else
		sender.setUsername(args[1]);
}