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

const std::map<int, Channel::ClientAndMod>& Channel::getClients() const {
	return _clients;
}

// setters
void Channel::setName(const std::string& name) {
	_name = name;
}

void Channel::setKey(const std::string& key) {
	_key = key;
}

void Channel::addClient(const Client& client, const std::string& key) {
	// Check if the client is already in the channel
	if (_clients.find(client.getSocket()) != _clients.end()) {
		return;
	}
	else if (!key.empty() && _key != key) {
		// wrong key
		// send ERR_BADCHANNELKEY
		return;
	}
	if (_clients.empty())
		_clients.insert(std::make_pair(client.getSocket(), ClientAndMod(client, '@')));
	else
		_clients.insert(std::make_pair(client.getSocket(), ClientAndMod(client, '\0')));
	if (client.getPollfd().revents & POLLOUT)
		Irc::getInstance().getServer()->send_join(client, *this);

	for (std::map<int, ClientAndMod>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second.client.getPollfd().revents & POLLOUT) {
			Irc::getInstance().getServer()->send_rpl_namreply(it->second.client, *this);
			Irc::getInstance().getServer()->send_rpl_endofnames(it->second.client, *this);
		}
	}
}