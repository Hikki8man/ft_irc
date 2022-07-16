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
	
}