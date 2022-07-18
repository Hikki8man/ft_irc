#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"

#include "sys/socket.h"
#include <poll.h>
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
#define CRLF "\r\n"
#define BUFFER_MAX 512

class Client;
class Channel;

class Server {
	public:
		Server();
		~Server();
		Server& operator=(const Server&);

		int run(int port);

		bool nickIsUsed(const std::string&);
		bool userIsUsed(const std::string&);

		// Send Reply to client
		void send_join(const Client&, const Channel&);
		void send_rpl_welcome(const Client&);
		void send_rpl_namreply(const Client&, const Channel&);
		void send_rpl_endofnames(const Client&, const Channel&);

		// Send Error Reply to client
		void send_err_nonicknamegiven(const Client&);
		void send_err_erroneusnickname(const Client&, const std::string&);
		void send_err_nicknameinuse(const Client&, const std::string&);
		void send_err_nosuchchannel(const Client&, const std::string&);
		void send_err_needmoreparams(const Client&, const std::string&);


		std::map<std::string, Channel>& getChannels();
		std::vector<pollfd>& getPollfds();
		const std::string getPrefix() const;
	private:
		Server(const Server&);

		int createServerSocket(int port);
		void setPrefix();
		int newConnection();
		int recvMsgFrom(SocketIt);


		void do_cmd(Client&);


		// Server socket && address
		SOCKET _srv_fd;
		struct sockaddr_in _srv_addr;
		std::string _srv_ip;
		std::string _prefix;

		// list of server and clients socket to poll 
		std::vector<pollfd> _sockets;

		// map of clients connected to server
		std::map<SOCKET, Client> _clients;

		// list of channels
		std::map<std::string, Channel> _channels;

};

#endif // SERVER_HPP