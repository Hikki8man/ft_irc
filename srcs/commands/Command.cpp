#include "../../includes/commands/Command.hpp"

Command::Command(Client& sender) : _sender(sender) {}

Command::Command(const std::string& name, const std::vector<std::string>& args, Client& sender) : _name(name), _args(args), _sender(sender) {}

const std::vector<std::string>& Command::getArgs() const {
	return _args;
}

const std::string& Command::getName() const {
	return _name;
}

const Client& Command::getSender() const {
	return _sender;
}

CommandExecutor *Command::parse(std::string& buffer) {
	// get the first full command and substract it from the client buffer
	size_t pos = buffer.find_first_of("\r\n");
	std::string cmd = buffer.substr(0, pos);
	buffer.erase(0, pos + 2);

	// Parse the command name
	pos = cmd.find_first_not_of(' ');
	if (pos != std::string::npos)
		cmd.erase(0, pos);
	pos = cmd.find_first_of(' ');
	if (pos != std::string::npos) {
		_name = cmd.substr(0, pos);
		cmd.erase(0, pos);
	}
	else
		_name = cmd;
	std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);
	CommandExecutor *executor = Irc::getInstance().getCommandManager()->getCommand(_name);
	// if cmd doesn't exist, stop parsing and return NULL
	if (!executor)
		return NULL;
	// if cmd exists, parse the args
	std::vector<std::string> args;
	const char* tmp = cmd.c_str();
	while (tmp) {
		while (*tmp == ' ') 
			++tmp;
		if (*tmp == ':') {
			++tmp;
			// later do case with the : at the end of the arg
			args.push_back(std::string(tmp));
			break;
		}
		size_t pos = std::string(tmp).find(" ");
		if (pos != std::string::npos) {
			args.push_back(std::string(tmp).substr(0, pos));
			tmp += pos;
		}
		else {
			args.push_back(std::string(tmp));
			tmp = NULL;
		}
	}
	_args = args;
	return executor;
}