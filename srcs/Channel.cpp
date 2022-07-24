#include "../includes/Channel.hpp"

/******************/
/*  Constructors  */
/******************/

Channel::Channel() : _name(""), _modes(""), _limit(0) {}

Channel::Channel(const std::string& name) : _name(name), _modes(""), _limit(0) {}

Channel::Channel(const Channel& other) {
	*this = other;
}

Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_name = other._name;
		_key = other._key;
		_clientsAndMode = other._clientsAndMode;
		_modes = other._modes;
		_limit = other._limit;
	}
	return *this;
}

Channel::~Channel() {}


/******************/
/*     Getters    */
/******************/

const std::string Channel::getName() const {
	return _name;
}

const std::string Channel::getKey() const {
	return _key;
}

const std::map<SOCKET, char> &Channel::getClientsAndMode() const {
	return _clientsAndMode;
}

char Channel::getClientMode(const SOCKET& sock) const {
	std::map<SOCKET, char>::const_iterator it = _clientsAndMode.find(sock);
	if (it == _clientsAndMode.end())
		return NONE;
	return it->second;
}

std::string Channel::getModes() const {
	return _modes;
}

const int Channel::getLimit() const {
	return _limit;
}

const std::string Channel::getTopic() const {
	return _topic;
}

const std::string Channel::getTopicCreatorAndWhen() const {
	return _topicCreatorAndWhen;
}

/******************/
/*     Setters    */
/******************/

void Channel::setName(const std::string& name) {
	_name = name;
}

void Channel::setKey(const std::string& key) {
	_key = key;
}

void Channel::setLimit(const int limit) {
	_limit = limit;
}

void Channel::setTopicCreatorAndWhen(const Client& client) {
	_topicCreatorAndWhen = client.getPrefix() + " " + intToString(time(NULL));
}

void Channel::setTopic(const Client& client, const std::string& topic) {
	if (topic.empty())
		_topic.clear();
	else
		_topic = topic;
		setTopicCreatorAndWhen(client);
	for (std::map<SOCKET, char>::iterator it = _clientsAndMode.begin(); it != _clientsAndMode.end(); it++) {
		std::string msg = "TOPIC " + _name + " :" + _topic;
		client.sendMessage(it->first, msg);
	}
}


/******************/
/*    Functions   */
/******************/

void Channel::clearTopic() {
	_topic.clear();
}

void Channel::addClient(Client& client, const std::string& key) {
	// Check if the client is already in the channel
	if (_clientsAndMode.find(client.getSocket()) != _clientsAndMode.end())
		return;
	else if (_key != key) {
		Irc::getInstance().getServer()->send_err_badchannelkey(client, _name);
		return;
	} else if (hasMode(LIMIT) && _clientsAndMode.size() >= _limit) {
		Irc::getInstance().getServer()->send_err_channelisfull(client, _name);
		return;
	} else if (hasMode(INVITE_ONLY) && !isInvited(client)) {
		Irc::getInstance().getServer()->send_err_inviteonlychan(client, _name);
		return;
	}
	if (_clientsAndMode.empty())
		_clientsAndMode.insert(std::make_pair(client.getSocket(), CHANNEL_OP));
	else
		_clientsAndMode.insert(std::make_pair(client.getSocket(), NONE));
	for (std::map<SOCKET, char>::iterator it = _clientsAndMode.begin(); it != _clientsAndMode.end(); ++it)
		client.sendMessage(it->first, std::string ("JOIN ") + _name);
	Irc::getInstance().getServer()->send_rpl_namreply(client, *this);
	Irc::getInstance().getServer()->send_rpl_endofnames(client, *this);
}

void Channel::removePartClient(const Client& client, const std::string& reason) {
	std::string msg = "PART " + _name; + " :" + reason;
	if (reason.empty())
		msg += " :" + reason;
	for (std::map<SOCKET, char>::iterator it = _clientsAndMode.begin(); it != _clientsAndMode.end(); ++it) {
		client.sendMessage(it->first, msg);
	}
	_clientsAndMode.erase(client.getSocket());
}

void Channel::insertClient(std::pair<SOCKET, char> client) {
	_clientsAndMode.insert(client);
}

void Channel::removeQuitClient(const Client& client) {
	removeInvite(client);
	_clientsAndMode.erase(client.getSocket());
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

void Channel::addInvite(const Client& client) {
	if (!isInvited(client))
		_invites.push_back(client.getSocket());
}

void Channel::removeInvite(const Client& client) {
	for (std::vector<SOCKET>::iterator it = _invites.begin(); it != _invites.end(); ++it) {
		if (*it == client.getSocket()) {
			_invites.erase(it);
			return;
		}
	}
}

const bool Channel::isInvited(const Client& client) const {
	return std::count(_invites.begin(), _invites.end(), client.getSocket()) == 1;
}

void Channel::setClientMode(const Client& client, char mode) {
	_clientsAndMode[client.getSocket()] = mode;
}