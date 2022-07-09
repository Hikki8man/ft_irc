#include "../includes/Server.hpp"

Client::Client() {}

Client::Client(int socket, struct sockaddr_in addr) : _socket(socket), _addr(addr) {}

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
	}
	return *this;
}

Client::~Client() {}

int Client::getSocket() const {
	return _socket;
}

struct sockaddr_in Client::getAddr() const {
	return _addr;
}

std::string Client::getNickname() const {
	return _nick;
}

std::string Client::getUsername() const {
	return _user;
}

void Client::setNickname(const std::string& nick) {
	_nick = nick;
}

void Client::setUsername(const std::string& user) {
	_user = user;
}
