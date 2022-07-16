#include "../../includes/commands/Command.hpp"

Command::Command(const std::string& name, const std::vector<std::string>& args, Client& sender) : _name(name), _args(args), _sender(sender) {}

std::vector<std::string>& Command::getArgs() {
	return _args;
}

const std::string& Command::getName() const {
	return _name;
}

const Client& Command::getSender() const {
	return _sender;
}