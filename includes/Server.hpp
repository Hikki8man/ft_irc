#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "NumericRepliesCode.hpp"
#include "CommandCode.hpp"

#include "sys/socket.h"
#include "poll.h"
#include <iostream>
#include <map>
#include <vector>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int
#define SocketIt std::vector<pollfd>::iterator
#define ClientIt std::map<SOCKET, Client>::iterator

#define RESTRICTED_CHARACTERS " ,*!@.$:#&"

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
		int recvMsgFrom(SocketIt);
		int sendMsgTo(const Client&, const std::string&);

		enum CommandCode getCommandCode(const std::string&);
		void do_cmd(pollfd&);
		void nickCmd(Client&, std::vector<std::string>&);

		std::vector<std::string> splitClientBuffer(Client& client);

		bool nickIsUsed(const std::string&);
		bool userIsUsed(const std::string&);

		// Server socket && address
		SOCKET _srv_fd;
		struct sockaddr_in _srv_addr;
		std::string _srv_ip;

		// list of server and clients socket to poll 
		std::vector<pollfd> _sockets;

		// map of clients connected to server
		std::map<SOCKET, Client> _clients;

		// list of channels
		// std::vector<Channel> _channels;

};

#endif // SERVER_HPP