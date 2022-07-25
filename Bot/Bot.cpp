#include "Bot.hpp"

Bot::Bot() {}

Bot::Bot(const std::string& serverAddress, const int& port, const std::string& password) : _serverAddress(serverAddress), _password(password), _port(port) {
	_nick = "Bot";
	_user = "Bot";
	_realName = "Bot";
}

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
	bool isRegistered = false;
	while (1) {
		int pollRet = poll(&_pollfd, 1, 0);

		if (pollRet < 0) { // handle errors
			perror("poll");
			break;
		}
		else if (pollRet > 0) {
			if (_pollfd.revents & POLLIN) {
				char buffer[1024] = {0};
				int n = recv(_pollfd.fd, buffer, 1024, 0);
				if (n > 0) {
					std::string message(buffer);
					std::cout << message << std::endl;
				}
				else if (n == 0) {
					std::cerr << "Connection closed" << std::endl;
					break;
				}
				else {
					perror("recv");
					break;
				}
			}
			else if (_pollfd.revents & POLLOUT && !isRegistered) {
				std::string message = "PASS " + _password + "\r\n";
				if (!_password.empty())
					sendMessage(message);
				message = "NICK " + _nick + "\r\n";
				sendMessage(message);
				message = "USER " + _user + " 0 * :" + _realName + "\r\n";
				sendMessage(message);
				isRegistered = true;
			}
		}
	}
	return EXIT_SUCCESS;
}

