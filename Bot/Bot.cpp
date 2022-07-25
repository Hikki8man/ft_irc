#include "Bot.hpp"

Bot::Bot() {}

Bot::Bot(const std::string& serverAddress, const int& port, const std::string& password) : _serverAddress(serverAddress), _password(password), _port(port) {}

Bot::Bot(const Bot& bot) {
	*this = bot;
}

Bot& Bot::operator=(const Bot& bot) {
	if (this == &bot) {
		_serverAddress = bot._serverAddress;
		_password = bot._password;
		_nick = bot._nick;
		_user = bot._user;
		_realName = bot._realName;
		_addr = bot._addr;
		_pollfd = bot._pollfd;
		_port = bot._port;
	}
	return *this;
}

Bot::~Bot() {}

void Bot::sendMessage(const std::string& message) const {
	if (_pollfd.revents & POLLOUT) {
		if (send(_pollfd.fd, message.c_str(), message.size(), 0) == -1)
			std::cerr << "Error while sending message" << std::endl;
	}
}

int Bot::connectToServer() {
	// Create socket
	if ((_pollfd.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Error: socket()" << std::endl;
		return EXIT_FAILURE;
	}
	// Set socket to NON BLOCK
	if (fcntl(_pollfd.fd, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl");
		return EXIT_FAILURE;
	}
	// Set address
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(_serverAddress.c_str());
	_addr.sin_port = htons(_port);
	_pollfd.events = POLLIN | POLLOUT;
	// Connect
	connect(_pollfd.fd, (struct sockaddr*)&_addr, sizeof(_addr));
	// if (connect(_pollfd.fd, (struct sockaddr*)&_addr, sizeof(_addr)) == -1) {
	// 	perror("connect");
	// 	return EXIT_FAILURE;
	// }
	return 0;
}

int Bot::run() {
	while (1) {
		
	}
}

