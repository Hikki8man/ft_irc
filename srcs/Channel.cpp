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

const Channel::ClientAndMod &Channel::getClientAndMod(const std::string& nick) const {
	for (std::map<int, Channel::ClientAndMod>::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.client.getNickname() == nick)
			return it->second;
	}
	return _clients.begin()->second;
}

const Client &Channel::getClient(const std::string& nick) const {
	for (std::map<int, Channel::ClientAndMod>::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.client.getNickname() == nick)
			return it->second.client;
	}
	return _clients.begin()->second.client;
}

std::vector<Channel::Mode> Channel::getModes() const {
	return _modes;
}


const Channel::Mode Channel::getModeById(char id) const {
	switch (id) {
		case 's':
			return secret;
		case 't':
			return protected_topic;
		case 'n':
			return no_external_msg;
		case 'i':
			return invite;
		case 'k':
			return key;
		case 'l':
			return limit;
		case 'm':
			return moderated;
		default:
			break;
	}
}

const std::string Channel::getModeId(Mode mode) const {
	switch (mode) {
		case secret:
			return "s";
		case protected_topic:
			return "t";
		case no_external_msg:
			return "n";
		case invite:
			return "i";
		case key:
			return "k";
		case limit:
			return "l";
		case moderated:
			return "m";
		default:
			break;
	}
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
	std::cout << "Channel: " << _name << " Key: " << _key << std::endl;
	if (_clients.find(client.getSocket()) != _clients.end()) {
		return;
	}
	else if (_key != key) {
		Irc::getInstance().getServer()->send_err_badchannelkey(client, _name);
		return;
	}
	if (_clients.empty())
		_clients.insert(std::make_pair(client.getSocket(), ClientAndMod(client, '@')));
	else
		_clients.insert(std::make_pair(client.getSocket(), ClientAndMod(client, '\0')));
	for (std::map<int, ClientAndMod>::iterator it = _clients.begin(); it != _clients.end(); ++it)
			Irc::getInstance().getServer()->send_join(it->second.client, client, *this);
	Irc::getInstance().getServer()->send_rpl_namreply(client, *this);
	Irc::getInstance().getServer()->send_rpl_endofnames(client, *this);
}

void Channel::removeClient(const Client& client, const std::string& reason) {
	for (std::map<int, ClientAndMod>::iterator it = _clients.begin(); it != _clients.end(); ++it)
			Irc::getInstance().getServer()->send_part(it->second.client, client, *this, reason);
	_clients.erase(client.getSocket());
}

const std::string Channel::getModeString() const {
	std::string modeString = "";
	for (std::vector<Mode>::const_iterator it = _modes.begin(); it != _modes.end(); ++it) {
		modeString += getModeId(*it);
	}
	return modeString;
}