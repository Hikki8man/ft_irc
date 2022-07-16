#include "commands/impl/UserCommand.hpp"
#include "Irc.hpp"

void UserCommand::execute(const Command& cmd, std::vector<std::string>& args, Client& sender)
{
	bool foundRealName = false;
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
		if (it->find(":", 0, 1) != std::string::npos) {
			foundRealName = true;
			std::string realName = it->c_str() + 1;
			++it;
			while (it != args.end()) {
				realName += " " + *it;
				++it;
			}
			sender.setRealName(realName);
			std::cout << "REALNAME: " << sender.getRealName() << std::endl;
			break;
		}
	}
	if (!foundRealName) {
		//send ERR_NEEDMOREPARAMS
		std::cout << "USER: no real name" << std::endl;
	}
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