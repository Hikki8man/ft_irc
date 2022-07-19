#include "../includes/Server.hpp"
#include "Irc.hpp"
#include "commands/CommandExecutor.hpp"
#include <iostream>

Server::Server() {}

Server::Server(const Server&) {}

Server& Server::operator=(const Server&) {
	return *this;
}

Server::~Server() {
	// TODO close all clients
}

bool Server::nickIsUsed(const std::string& nick) {
	for (ClientIt it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getNickname() == nick)
			return true;
	}
	return false;
}

// Getter ===========================================================================

std::vector<pollfd>& Server::getPollfds() {
	return _sockets;
}

const std::string Server::getPrefix() const {
	return _prefix;
}

const std::string Server::getIp() const {
	return _srv_ip;
}

// Setter ===========================================================================

void Server::setPrefix() {
	_prefix = std::string(":") + inet_ntoa(_srv_addr.sin_addr);
}

// Methods ==========================================================================

void Server::do_cmd(Client& sender) {

	std::cout << "msg received: " << sender.getBuffer() << std::endl;
	while (sender.getBuffer().find("\r\n") != std::string::npos) {

		Command cmd(sender);
		CommandExecutor *executor = cmd.parse(sender.getBuffer());

		if (executor) {
			if (executor->isRegisteredOnly() && !sender.isRegistered())
				Irc::getInstance().getServer()->send_err_notregistered(sender);
			else
				executor->execute(cmd, sender);
		}
		else {
			std::cout << "Command not found" << std::endl;
		}

		if (!sender.isRegistered() && !sender.getNickname().empty() && !sender.getUsername().empty()) {
			sender.setRegistered(true);
			sender.setPrefix();
			std::cout << "Client registered:" << std::endl;
			std::cout << "Nickname: " << sender.getNickname() << std::endl;
			std::cout << "Username: " << sender.getUsername() << std::endl;
			send_rpl_welcome(sender);
		}
		std::cout << "--------------------------------------" << std::endl;
	}
}

// Server socket =========================================================================

int Server::createServerSocket(int port) {
	// Creating socket file descriptor
	if ((_srv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		return EXIT_FAILURE;
	}
	// Set socket options
	int opt = 1;
	if (setsockopt(_srv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		return EXIT_FAILURE;
	}

	// Set socket to NON BLOCK
	if (fcntl(_srv_fd, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl");
		return EXIT_FAILURE;
	}

	// Setting server address, port and family
	_srv_addr.sin_family = AF_INET;
	_srv_addr.sin_addr.s_addr = INADDR_ANY;
	_srv_addr.sin_port = htons(port);

	// Binding socket to the address and port
	if (bind(_srv_fd, (struct sockaddr *)&_srv_addr, sizeof(_srv_addr)) < 0) {
		perror("bind failed");
		return EXIT_FAILURE;
	}

	// Listen for incoming connections
	if (listen(_srv_fd, 15) < 0) {
		perror("listen");
		return EXIT_FAILURE;
	}
	// Add server socket to poll
	pollfd serverSocket;
	serverSocket.fd = _srv_fd;
	serverSocket.events = POLLIN;
	_srv_ip = inet_ntoa(_srv_addr.sin_addr);
	_sockets.push_back(serverSocket);
	setPrefix();

	return 0;
}

int Server::newConnection() {
	std::cout << "creating new connection" << std::endl;
	SOCKET new_socket;
	struct sockaddr_in client_addr = {0};
	socklen_t client_addr_len = sizeof(client_addr);


	if ((new_socket = accept(_srv_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
		perror("accept");
		return EXIT_FAILURE;
	}

	// Add new socket to poll
	pollfd newSocket;
	newSocket.fd = new_socket;
	newSocket.events = POLLIN | POLLOUT;
	_sockets.push_back(newSocket);

	std::cout << inet_ntoa(client_addr.sin_addr) << " connected" << std::endl;
	

	// Add new client to clients list, hummmm
	Client newClient(new_socket, client_addr);
	_clients[new_socket] = newClient;

	return 0;
}

int Server::recvMsgFrom(SocketIt socket) {
	char buffer[BUFFER_MAX] = {0};
	Client& sender = _clients[socket->fd];
	int n;
	if ((n = recv(socket->fd, buffer, BUFFER_MAX, 0)) < 0) {
		// _clients.erase(socket->fd);
		// _sockets.erase(socket); ??
		perror("recv");
		return 1;
	}
	if (n == 0) {
		// need to remove client from clients list of channels
		std::cout << sender.getNickname() << " disconnected" << std::endl;
		_clients.erase(socket->fd);
		_sockets.erase(socket);
		return 0;
	}
	sender.setBuffer(buffer);
	do_cmd(sender);
	return 1;
}

std::map<std::string, Channel>& Server::getChannels() {
	return _channels;
}

int Server::run(int port) {
	if (createServerSocket(port) < 0) {
		return EXIT_FAILURE;
	}
	while (1) {
		// Check for activity on all sockets
		int pollRet = poll(&_sockets[0], _sockets.size(), 0);

		if (pollRet < 0) { // handle errors
			perror("poll");
			break;
		}
		else if (pollRet > 0) {
			for (SocketIt it = _sockets.begin(); it != _sockets.end(); ++it) {
				if (it->revents & POLLIN) {
					if (it->fd == _srv_fd) {
						newConnection();
						break;
					}
					else if (!_clients.empty()) {
						if (!recvMsgFrom(it)) {
							break;
						}
					}
				}
			}
		}
	}
	return 0;
}