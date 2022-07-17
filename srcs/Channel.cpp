#include "../includes/Channel.hpp"

Channel::Channel() : _name("") {}

Channel::Channel(const std::string& name) : _name(name) {}

Channel::Channel(const Channel& other) {
	*this = other;
}

Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_name = other._name;
		_clients = other._clients;
	}
	return *this;
}

Channel::~Channel() {}

// Struct ClientAndMod
Channel::ClientAndMod::ClientAndMod(const Client& client, char mod) : client(client), mod(mod) {}

// getters
const std::string Channel::getName() const {
	return _name;
}

const std::string Channel::getKey() const {
	return _key;
}

// setters
void Channel::setName(const std::string& name) {
	_name = name;
}

void Channel::setKey(const std::string& key) {
	_key = key;
}

void Channel::addClient(const Client& client) {
	if (_clients.find(client.getSocket()) != _clients.end()) {
		// client already in channel
		return;
	}
	if (_clients.empty())
		_clients.insert(std::make_pair(client.getSocket(), ClientAndMod(client, '@')));
	else
		_clients.insert(std::make_pair(client.getSocket(), ClientAndMod(client, '\0')));
	// send join message to client
	// send NAMES && end of NAMES message to client
}