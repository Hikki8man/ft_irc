#include "../includes/Server.hpp"



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

int Server::sendMsgTo(const Client& client, const std::string& code) {
	std::string msg = ":" + _srv_ip + " " + code + " " + client.getNickname() + " :"; // probably need to change that in future
	if (code == RPL_WELCOME) {
		msg += "Welcome to the IRC server " + client.getNickname() + "!" + "\r\n";
		std::cout << msg << std::endl;
		return send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
	return -1;
}

void Server::do_cmd(pollfd& sock) {
	Client& client = _clients[sock.fd];
	std::string buffer = client.getBuffer();
	std::vector<std::string> cmd_args;

	buffer.erase(buffer.find_last_of("\r")); // \n ?

	// split the buffer into cmd and args
	const char* tmp = buffer.c_str();
	while (tmp) {
		while (*tmp == ' ') 
			++tmp;
		size_t pos = std::string(tmp).find(" ");
		if (pos != std::string::npos) {
			cmd_args.push_back(std::string(tmp).substr(0, pos));
			tmp += pos;
		}
		else {
			cmd_args.push_back(std::string(tmp));
			tmp = NULL;
		}
	}
	client.getBuffer().clear();
	//=====================

	// for (std::vector<std::string>::iterator it = cmd_args.begin(); it != cmd_args.end(); ++it) {
	// 	std::cout << "in cmd_args: " << *it << std::endl;
	// }
	
	int code = getCommandCode(cmd_args[0]);
	switch (code) {
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
		{
			bool foundRealName = false;
			for (std::vector<std::string>::iterator it = cmd_args.begin(); it != cmd_args.end(); ++it) {
				if (it->find(":", 0, 1) != std::string::npos) {
					foundRealName = true;
					std::string realName = it->c_str() + 1;
					++it;
					while (it != cmd_args.end()) {
						realName += " " + *it;
						++it;
					}
					client.setRealName(realName);
					std::cout << "REALNAME: " << client.getRealName() << std::endl;
					break;
				}
			}
			if (!foundRealName) {
				//send ERR_NEEDMOREPARAMS
				std::cout << "USER: no real name" << std::endl;
			}
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
		}
		case UNKNOWN:
			// send error message
			std::cout << "Unknown command" << std::endl;
			break;
		default:
			break;
	}
	std::cout << "Client nickname: " << client.getNickname() << std::endl;
	std::cout << "Client username: " << client.getUsername() << std::endl;

	if (!client.isRegistered() && !client.getNickname().empty() && !client.getUsername().empty()) {
		client.setRegistered(true);
		std::cout << "Client registered" << std::endl;
		if (sock.revents & POLLOUT)
			sendMsgTo(client, RPL_WELCOME);
		else {
			std::cout << "Client not ready" << std::endl;
		}
		//send welcome message + check pollout
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
	_srv_ip = inet_ntoa(_srv_addr.sin_addr);
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
		std::cout << client.getNickname() << " disconnected" << std::endl;
		_clients.erase(socket->fd);
		_sockets.erase(socket);
		return 0;
	}
	client.setBuffer(buffer);
	std::string msg = client.getBuffer();
	// std::cout << "msg: " << msg << std::endl;
	if (msg.find("\r\n") == std::string::npos) {
		std::cout << "msg not complete in:" << std::endl;
		return 1;
	}
	else {
		std::cout << "msg: " << msg << std::endl;
		do_cmd(*socket);
	}
	// std::cout << "Received: " << buffer << std::endl;
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