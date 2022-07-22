#include "../includes/Client.hpp"
#include "Irc.hpp"

Client::Client() : _socket(), _addr(), _registered(false) {}

Client::Client(int socket, struct sockaddr_in addr) : _socket(socket), _addr(addr), _registered(false) {}

Client::Client(const Client& other) {
	*this = other;
}

Client& Client::operator=(const Client& other) {
	if (this != &other) {
		_nick = other._nick;
		_user = other._user;
		_realName = other._realName;
		_socket = other._socket;
		_addr = other._addr;
		_bufferStocked = other._bufferStocked;
		_registered = other._registered;
		_channels = other._channels;
		_prefix = other._prefix;
		_ip = other._ip;
	}
	return *this;
}

Client::~Client() {}

// GETTERS
int Client::getSocket() const {
	return _socket;
}

struct sockaddr_in Client::getAddr() const {
	return _addr;
}

const std::string Client::getNickname() const {
	return _nick;
}

const std::string Client::getUsername() const {
	return _user;
}

const std::string Client::getRealName() const {
	return _realName;
}

const pollfd& Client::getPollfd() const {
	SocketIt it = Irc::getInstance().getServer()->getPollfds().begin();
	for (; it != Irc::getInstance().getServer()->getPollfds().end(); it++) {
		if (it->fd == _socket)
			return *it;
	}
	return *it;
}

std::string& Client::getBuffer() {
	return _bufferStocked;
}

const std::string Client::getPrefix() const {
	return _prefix;
}

std::map<std::string, const Channel&>& Client::getChannels() {
	return _channels;
}

const std::string Client::getIp() const {
	return _ip;
}

// SETTERS
void Client::setNickname(const std::string& nick) {
	_nick = nick;
}

void Client::setRealName(const std::string& realName) {
	_realName = realName;
}

void Client::setUsername(const std::string& user) {
	_user = user;
}

void Client::setBuffer(const std::string& buffer) {
	_bufferStocked += buffer;
}

void Client::setRegistered(bool registered) {
	_registered = registered;
}

void Client::setPrefix() {
	_prefix = ":" + _nick + "!" + _user + "@" + getIp();
}

void Client::setIp(const std::string& ip) {
	_ip = ip;
}

// Methods

bool Client::isRegistered() const {
	return _registered && isLogged();
}

void Client::setLogged(bool logged) {
	_logged = logged;
}

const bool Client::isLogged() const {
	return _logged || Irc::getInstance().getServer()->getPassword().length() == 0;
}

bool Client::isInChannel(const Channel& channel) const {
	return _channels.find(channel.getName()) != _channels.end();
}

void Client::addChannel(const Channel& channel) {
	_channels.insert(std::make_pair(channel.getName(), channel));
	// _channels[channel.getName()] = channel;
}

void Client::removeChannel(Channel& channel) {
	_channels.erase(channel.getName());
}
