#include "../includes/Server.hpp"
#include "Irc.hpp"
#include "commands/CommandExecutor.hpp"

/******************/
/*  Constructors  */
/******************/

Server::Server() : _password("") {}

Server::Server(const Server&) {}

Server& Server::operator=(const Server&) {
	return *this;
}

Server::~Server() {
	for (SocketIt it = _sockets.begin(); it != _sockets.end(); ++it) {
		close(it->fd);
	}
}

bool Server::nickIsUsed(const std::string& nick) {
	for (ClientIt it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getNickname() == nick)
			return true;
	}
	return false;
}

bool Server::userIsUsed(const std::string& user) {
	for (ClientIt it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getUsername() == user)
			return true;
	}
	return false;
}


/******************/
/*     Getters    */
/******************/

std::vector<pollfd>& Server::getPollfds() {
	return _sockets;
}

const std::string Server::getPrefix() const {
	return _prefix;
}

std::map<std::string, Channel>& Server::getChannels() {
	return _channels;
}

std::map<SOCKET, Client>& Server::getClients() {
	return _clients;
}

Client &Server::findClientByName(const std::string& nick) {
	for (ClientIt it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getNickname() == nick)
			return it->second;
	}
	return _clients.begin()->second;
}

Client &Server::getClientBySocket(SOCKET socket) {
	std::map<int, Client>::iterator client = _clients.find(socket);
	if (client == _clients.end())
		return _clients.begin()->second;
	return client->second;
}

const std::string Server::getIp() const {
	return _srv_ip;
}

const std::string Server::getPassword() const {
	return _password;
}

SOCKET Server::getSocket() const {
	return _srv_fd;
}

/******************/
/*     Setters    */
/******************/

void Server::setPrefix() {
	_prefix = std::string(":") + _srv_ip;
}

void Server::setPassword(const std::string& password) {
	_password = password;
}


/******************/
/*    Functions   */
/******************/

void Server::sendMessage(int toSend, const std::string& args, bool prefix) const {
	Client& toSendTo = Irc::getInstance().getServer()->getClientBySocket(toSend);
	if (toSendTo.getPollfd().revents & POLLOUT) {
		std::string msg;
		if (prefix == true)
			msg = getPrefix() + " ";
		msg += args + CRLF;
		int ret = send(toSendTo.getSocket(), msg.c_str(), msg.length(), 0);
		if (ret == -1)
			std::cerr << "Error while sending message to " << toSendTo.getNickname() << std::endl;
	}
}

void Server::sendMessage(Client &toSendTo, const std::string& args, bool prefix) const {
	if (toSendTo.getPollfd().revents & POLLOUT) {
		std::string msg;
		if (prefix == true)
			msg = getPrefix() + " ";
		msg += args + CRLF;
		int ret = send(toSendTo.getSocket(), msg.c_str(), msg.length(), 0);
		if (ret == -1)
			std::cerr << "Error while sending message to " << toSendTo.getNickname() << std::endl;
	}
}

void Server::do_cmd(Client& sender) {
	std::cout << "msg received: " << sender.getBuffer() << std::endl;
	while (sender.getBuffer().find("\r\n") != std::string::npos) {

		Command cmd;
		CommandExecutor *executor = cmd.parse(sender.getBuffer());

		if (executor) {
			if (cmd.getName() != "PASS" && cmd.getName() != "QUIT" && !sender.isLogged() && getPassword().length() > 0) {
				sendMessage(sender, "NOTICE PASS :You need to identify with PASS command first.");
			}
			else if (executor->isRegisteredOnly() && !sender.isRegistered())
				Irc::getInstance().getServer()->send_err_notregistered(sender);
			else
				executor->execute(cmd, sender);
		}
		else if (cmd.getName() != "CAP") {
			send_err_unknowncommand(sender, cmd.getName());
		}

		if (!sender.isRegistered() && !sender.getNickname().empty() && !sender.getUsername().empty() && sender.isLogged()) {
			sender.setRegistered(true);
			sender.setPrefix();
			std::cout << "Client registered:" << std::endl;
			std::cout << "Nickname: " << sender.getNickname() << std::endl;
			std::cout << "Username: " << sender.getUsername() << std::endl;
			std::cout << "Logged: " << sender.isLogged() << " registered: " << sender.isRegistered() << std::endl;
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
	_srv_ip = SERVER_ADDR;
	_sockets.push_back(serverSocket);
	setPrefix();

	return 0;
}

int Server::newConnection() {
	std::cout << "creating new connection" << std::endl;
	SOCKET new_socket;
	struct sockaddr_in client_addr = {0, 0, 0, {0}, {0}};
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

	// Add new client to clients list
	Client newClient(new_socket, client_addr);
	newClient.setIp(inet_ntoa(client_addr.sin_addr));
	_clients[new_socket] = newClient;

	std::cout << newClient.getIp() << " connected" << std::endl;

	return 0;
}

int Server::recvMsgFrom(SocketIt socket) {
	char buffer[BUFFER_MAX] = {0};
	Client& sender = _clients[socket->fd];
	int n;
	if ((n = recv(socket->fd, buffer, BUFFER_MAX - 2, 0)) < 0) {
		_clients.erase(socket->fd);
		perror("recv");
		return 0;
	}
	if (n == 0) {
		std::cout << sender.getIp() << " disconnected" << std::endl;
		Command cmd;
		sender.getBuffer().clear();
		sender.setBuffer("QUIT :Remote host closed the connection\r\n");
		CommandExecutor *executor = cmd.parse(sender.getBuffer());
		executor->execute(cmd, sender);
		_clients.erase(socket->fd);
		return 0;
	}
	std::string buf(buffer);
	if (buf.size() == BUFFER_MAX - 2)
		buf += CRLF;
	sender.setBuffer(buf);
	do_cmd(sender);
	return 1;
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
				if ((it->fd != _srv_fd && _clients.find(it->fd) == _clients.end()) || it->revents & POLLERR) {
					close(it->fd);
					_sockets.erase(it);
					break;
				}
				else if (it->revents & POLLIN) {
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