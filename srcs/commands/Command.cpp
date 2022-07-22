#include "../../includes/commands/Command.hpp"

Command::Command() : _executor(NULL) {}

Command::Command(const std::string& name, const std::vector<std::string>& args) : _name(name), _args(args) {}

const std::vector<std::string>& Command::getArgs() const {
	return _args;
}

const std::string& Command::getName() const {
	return _name;
}

CommandExecutor *Command::getExecutor() const {
	return _executor;
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
	else {
		_name = cmd;
		cmd.clear();
	}
	std::transform(_name.begin(), _name.end(), _name.begin(), ::toupper);
	CommandExecutor *executor = Irc::getInstance().getCommandManager()->getCommand(_name);
	// if cmd doesn't exist, stop parsing and return NULL
	if (!executor)
		return NULL;
	
	// if cmd exists, parse the args
	std::vector<std::string> args;
	while (!cmd.empty()) {
		pos = cmd.find_first_not_of(' ');
		if (pos != std::string::npos)
			cmd.erase(0, pos);
		else
			break;
		if (cmd.at(0) == ':') {
			cmd.erase(0, 1);
			// later do case with the : at the end of the arg ou pas
			args.push_back(cmd);
			break;
		}
		pos = cmd.find(" ");
		if (pos != std::string::npos) {
			args.push_back(cmd.substr(0, pos));
			cmd.erase(0, pos);
		}
		else {
			args.push_back(cmd);
			cmd.clear();
		}
	}
	_args = args;
	_executor = executor;
	return executor;
}