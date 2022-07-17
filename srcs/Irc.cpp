#include "Irc.hpp"

Irc& Irc::getInstance() {
	return *Irc::instance;
}

Irc::Irc() {
	Irc::instance = this;
}

Server* Irc::getServer() {
	return server;
}

void Irc::setServer(Server* server) {
	this->server = server;
}

CommandManager* Irc::getCommandManager() {
	return commandManager;
}

void Irc::setCommandManager(CommandManager* commandManager) {
	this->commandManager = commandManager;
}

Irc *Irc::instance = NULL;