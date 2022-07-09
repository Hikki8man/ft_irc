#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

#include "sys/socket.h"
#include "poll.h"
#include <iostream>
#include <map>
#include <vector>

#define SOCKET int

class Server {
	public:
		Server();
		~Server();

		int run(int port);

	private:
		Server(const Server&);
		Server& operator=(const Server&);

		int createServerSocket(int port);
		int newConnection();

		// Server socket && address
		SOCKET _srv_fd;
		struct sockaddr_in _srv_addr;

		// vector of server and clients socket to poll 
		std::vector<pollfd> _sockets;

		// map of clients connected to server
		std::map<SOCKET, Client> _clients;

};

#endif // SERVER_HPP