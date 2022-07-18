#include "commands/impl/UserCommand.hpp"
#include "Irc.hpp"

void UserCommand::execute(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	bool foundRealName = false;

	sender.setRealName(args[args.size() - 1]); // todo: redo
	// for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
	// 	if (it->find(":", 0, 1) != std::string::npos) {
	// 		foundRealName = true;
	// 		std::string realName = it->c_str() + 1;
	// 		++it;
	// 		while (it != args.end()) {
	// 			realName += " " + *it;
	// 			++it;
	// 		}
	// 		sender.setRealName(realName);
	// 		std::cout << "REALNAME: " << sender.getRealName() << std::endl;
	// 		break;
	// 	}
	// }
	// if (!foundRealName) {
	// 	//send ERR_NEEDMOREPARAMS
	// 	std::cout << "USER: no real name" << std::endl;
	// }
	// verify of characters are valid
	// ---
	// verify if User already registered
	if (sender.isRegistered()) {
		// send error message
		std::cout << "User already registered" << std::endl;
	}
	// verify if User is already used
	else if (Irc::getInstance().getServer()->userIsUsed(args[0])) {
		// send error message
		std::cout << "Username already used" << std::endl;
	} else
		sender.setUsername(args[0]);
}