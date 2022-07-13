#include "../includes/Server.hpp"

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

std::string& Client::getNickname() {
	return _nick;
}

std::string& Client::getUsername() {
	return _user;
}

std::string& Client::getBuffer() {
	return _bufferStocked;
}

void Client::setNickname(const std::string& nick) {
	_nick = nick;
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

bool Client::isRegistered() const {
	return _registered;
}