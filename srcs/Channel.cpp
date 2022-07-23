#include "../includes/Channel.hpp"

Channel::Channel() : _name(""), _modes(""), _limit(0) {}

Channel::Channel(const std::string& name) : _name(name), _modes(""), _limit(0) {}

Channel::Channel(const Channel& other) {
	*this = other;
}

Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_name = other._name;
		_key = other._key;
		_clientsAndMod = other._clientsAndMod;
		_modes = other._modes;
		_limit = other._limit;
	}
	return *this;
}

Channel::~Channel() {}


// getters
const std::string Channel::getName() const {
	return _name;
}

const std::string Channel::getKey() const {
	return _key;
}

std::map<SOCKET, char> &Channel::getClientsAndMod() {
	return _clientsAndMod;
}

// const Channel::ClientAndMod &Channel::getClientAndMod(const std::string& nick) const {
// 	for (std::map<int, Channel::ClientAndMod>::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
// 		if (it->second.client.getNickname() == nick)
// 			return it->second;
// 	}
// 	return _clients.begin()->second;
// }

// const Client &Channel::findClientByName(const std::string& nick) const {
// 	for (std::map<int, std::pair<Client&, char> >::const_iterator it = _clientsAndMod.begin(); it != _clientsAndMod.end(); it++) {
// 		if (it->second.client.getNickname() == nick)
// 			return it->second.client;
// 	}
// 	return _clients.begin()->second.client;
// }

std::string Channel::getModes() const {
	return _modes;
}

const int Channel::getLimit() const {
	return _limit;
}

// setters
void Channel::setName(const std::string& name) {
	_name = name;
}

void Channel::setKey(const std::string& key) {
	_key = key;
}

void Channel::setLimit(const int limit) {
	_limit = limit;
}

void Channel::addClient(Client& client, const std::string& key) {
	// Check if the client is already in the channel
	if (_clientsAndMod.find(client.getSocket()) != _clientsAndMod.end())
		return;
	else if (_key != key) {
		Irc::getInstance().getServer()->send_err_badchannelkey(client, _name);
		return;
	} else if (hasMode(LIMIT) && _clientsAndMod.size() >= _limit) {
		Irc::getInstance().getServer()->send_err_channelisfull(client, _name);
		return;
	}
	if (_clientsAndMod.empty())
		_clientsAndMod.insert(std::make_pair(client.getSocket(), '@'));
	else
		_clientsAndMod.insert(std::make_pair(client.getSocket(), '\0'));
	for (std::map<SOCKET, char>::iterator it = _clientsAndMod.begin(); it != _clientsAndMod.end(); ++it)
		client.sendMessage(it->first, std::string ("JOIN ") + _name);
	Irc::getInstance().getServer()->send_rpl_namreply(client, *this);
	Irc::getInstance().getServer()->send_rpl_endofnames(client, *this);
}

void Channel::removePartClient(const Client& client, const std::string& reason) {
	std::string msg = "PART " + _name; + " :" + reason;
	if (reason.empty())
		msg += " :" + reason;
	for (std::map<SOCKET, char>::iterator it = _clientsAndMod.begin(); it != _clientsAndMod.end(); ++it) {
		client.sendMessage(it->first, msg);
	}
	_clientsAndMod.erase(client.getSocket());
}

void Channel::removeQuitClient(const Client& client) {
	_clientsAndMod.erase(client.getSocket());
}

void Channel::addMode(char mode) {
	_modes.push_back(mode);
}

void Channel::removeMode(char mode) {
	_modes.erase(_modes.find(mode), 1);
}

const bool Channel::hasMode(char mode) const {
	return _modes.find(mode) != std::string::npos;
}
