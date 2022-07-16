#include "../includes/Server.hpp"
#include "Irc.hpp"
#include "commands/CommandExecutor.hpp"
#include <algorithm>
#include <iostream>


Server::Server() {}

Server::Server(const Server&) {}

Server& Server::operator=(const Server&) {
	return *this;
}

Server::~Server() {}

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

CommandCode Server::getCommandCode(const std::string& cmd) {
	if (cmd == "NICK") {
		return NICK;
	} else if (cmd == "USER") {
		return USER;
	} else if (cmd == "JOIN") {
		return JOIN;
	} else if (cmd == "PART") {
		return PART;
	} else if (cmd == "QUIT") {
		return QUIT;
	} else if (cmd == "PRIVMSG") {
		return PRIVMSG;
	} else if (cmd == "PING") {
		return PING;
	} else if (cmd == "PONG") {
		return PONG;
	} else {
		return UNKNOWN;
	}
}

void Server::do_cmd(SOCKET sock) {
	Client& client = _clients[sock];
	std::string buffer = client.getBuffer();
	buffer.erase(buffer.find_last_of("\r\n"));
	std::vector<std::string> cmd_args;

	// split the buffer into cmd and args
	const char* tmp = buffer.c_str();
	while (tmp) {
		while (*tmp == ' ')
			++tmp;
		size_t pos = std::string(tmp).find(" ");
		if (pos != std::string::npos) {
			cmd_args.push_back(std::string(tmp).substr(0, pos));
			std::cout << "tmp: " << tmp << std::endl;
			tmp += pos;
		}
		else {
			cmd_args.push_back(std::string(tmp));
			tmp = NULL;
		}
	}
	client.getBuffer().clear();
	//=====================

	for (std::vector<std::string>::iterator it = cmd_args.begin(); it != cmd_args.end(); ++it) {
		std::cout << "in cmd_args: " << *it << std::endl;
	}
	
	int code = getCommandCode(cmd_args[0]);
	std::cout << "COMMAND: " << buffer << "--------------------------------------"  <<std::endl;

	std::transform(cmd_args[0].begin(), cmd_args[0].end(), cmd_args[0].begin(), ::tolower);

	Command cmd(cmd_args[0], cmd_args, client);

	CommandExecutor *executor = Irc::getInstance().getCommandManager().getCommand(cmd_args[0]);

	if (executor) {
		executor->execute(cmd, cmd_args, client);
	}
	else {
		std::cout << "Command not found" << std::endl;
	}
	/* switch (code)	{
		case NICK:
		{
			// verify of characters are valid
			std::string restrict(" ,*!@.$:#&");
			if (cmd_args[1].find_first_of(restrict) != std::string::npos) {
				//send proper error message
				std::cout << "NICK: invalid characters" << std::endl;
				break;
			}
			// ---
			// verify if nick is already used
			if (nickIsUsed(cmd_args[1])) {
				// send error message
				std::cout << "Nickname already used" << std::endl;
			} else
				client.setNickname(cmd_args[1]);
			break;

		}
		case USER:
			// verify of characters are valid
			// ---
			// verify if User already registered
			if (client.isRegistered()) {
				// send error message
				std::cout << "User already registered" << std::endl;
			}
			// verify if User is already used
			else if (userIsUsed(cmd_args[1])) {
				// send error message
				std::cout << "Username already used" << std::endl;
			} else
				client.setUsername(cmd_args[1]);
			break;
		case UNKNOWN:
			// send error message
			std::cout << "Unknown command" << std::endl;
			break;
		default:
			break;
	} */
	std::cout << "Client nickname: " << client.getNickname() << std::endl;
	std::cout << "Client username: " << client.getUsername() << std::endl;

	if (!client.isRegistered() && !client.getNickname().empty() && !client.getUsername().empty()) {
		client.setRegistered(true);
		std::cout << "Client registered" << std::endl;
		//send welcome message
	}
	cmd_args.clear();
	std::cout << "--------------------------------------" << std::endl;
}

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
	_sockets.push_back(serverSocket);

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
	_clients[new_socket] = newClient;;

	std::cout << "new client socket: " << newSocket.fd << std::endl;

	
	// std::cout << "New connection: " << newClient.getNickname() << std::endl;

	return 0;
}

int Server::recvMsgFrom(SocketIt socket) {
	char buffer[1024] = {0};
	Client& client = _clients[socket->fd];
	int n;
	if ((n = recv(socket->fd, buffer, 1024, 0)) < 0) {
		// _clients.erase(socket->fd);
		// _sockets.erase(socket); ??
		perror("recv");
		return 1;
	}
	if (n == 0) {
		std::cout << "Client disconnected" << std::endl;
		_clients.erase(socket->fd);
		_sockets.erase(socket);
		return 1;
	}
	client.setBuffer(buffer);
	std::string msg = client.getBuffer();
	// std::cout << "msg: " << msg << std::endl;
	if (msg.find("\r\n", msg.length() - 2) == std::string::npos) {
		std::cout << "msg not complete in: " << msg << std::endl;
		return 0;
	}
	else {
		std::cout << "msg: " << msg << std::endl;
		do_cmd(socket->fd);
		// do_cmd
		// peut il y avoir plusieurs commandes dans la même ligne ?
		// peut il y avoir plusieurs espaces ?
		// trop de questions pour le moment
		// zzzzz ZZZZ
	}
	// std::cout << "Received: " << buffer << std::endl;
	return 0;
}

int Server::run(int port) {
	if (createServerSocket(port) < 0) {
		return EXIT_FAILURE;
	}
	while (1) {
		int pollRet = poll(&_sockets[0], _sockets.size(), 0);

		if (pollRet < 0) { // handle errors
			perror("poll");
			break;
		}
		else if (pollRet > 0) {

			for (SocketIt it = _sockets.begin(); it != _sockets.end(); ++it) {
				if (it->revents & POLLIN) {
					// std::cout << "actual fd: " << it->fd << std::endl;
					// std::cout << "sockets size: " << _sockets.size() << std::endl;
					// for (ClientIt it2 = _clients.begin(); it2 != _clients.end(); ++it2) {
					// 	std::cout << "client socket: " << it2->second.getSocket() << std::endl;
					// }
					// for (SocketIt it2 = _sockets.begin(); it2 != _sockets.end(); ++it2) {
					// 	std::cout << "sockets: " << it2->fd << std::endl;
					// }
					if (it->fd == _srv_fd) {
						newConnection();
						break;
					}
					else if (!_clients.empty()) {
						if (recvMsgFrom(it) < 0) {
							break;
						}
						// ClientIt clientIt = _clients.find(it->fd);
						// if (clientIt == _clients.end()) {
						// 	_sockets.erase(it);
						// 	std::cout << "problem" << std::endl;
						// 	break;
						// }
						// char buffer[1024] = {0};
						// int ret = recv(it->fd, buffer, 1023, 0);
						// if (ret > 0) {
						// 	buffer[ret] = '\0';
						// 	std::cout << "Client " << it->fd << ": " << buffer << std::endl;
						// }
						// else if (ret == 0) {
						// 	std::cout << "Client " << it->fd << " disconnected" << std::endl;
						// 	_clients.erase(it->fd);
						// 	// std::cout << "client after erase: " << _clients.size() << std::endl;
						// 	it = _sockets.erase(it);
						// 	// std::cout << "sockets after erase: " << _sockets.size() << std::endl;
						// 	break;
						// }
						// else {
						// 	perror("recv");
						// 	break;
						// }
					}
				}
			}
		}
	}
	return 0;
}