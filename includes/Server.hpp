#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
// #include "commands/impl/NoticeCommand.hpp"

#include "sys/socket.h"
#include <poll.h>
#include <iostream>
#include <map>
#include <vector>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SOCKET int
#define SocketIt std::vector<pollfd>::iterator
#define ClientIt std::map<SOCKET, Client>::iterator

#define CRLF "\r\n"
#define BUFFER_MAX 512
#define USERLEN 18
#define NICKLEN 31

class Client;
class Channel;

class Server {
	public:
		Server();
		~Server();
		Server& operator=(const Server&);

		int run(int port);

		bool nickIsUsed(const std::string&);

		// void send_join(SOCKET sender, SOCKET joiner, const Channel&);
		// void send_part(const Client&, const Client&, const Channel&, const std::string&);
		// void send_quit(const Client&, SOCKET receiver, const std::string&);
		// void send_privmsg(const Client&, const Client&, const std::string&, const std::string&);
		// void send_notice(const std::string&, const Client&, const std::string&, const std::string&);
		void send_error(const Client&, const std::string&);

		// Send Reply to client
		void sendMessage(int toSend, const std::string& message, bool prefix = true) const;
		void sendMessage(Client &toSend, const std::string& message, bool prefix = true) const;

		void send_rpl_welcome(const Client&);
		void send_rpl_namreply(const Client&, const Channel&);
		void send_rpl_endofnames(const Client&, const Channel&);
		void send_rpl_channelmodeis(const Client&, const Channel&);
		void send_rpl_topic(const Client&, const Channel&);
		void send_rpl_notopic(const Client&, const Channel&);
		void send_rpl_topicwhotime(const Client&, const Channel&);

		// Send Error Reply to client
		void send_err_nosuchnick(const Client&, const std::string&);
		void send_err_nonicknamegiven(const Client&);
		void send_err_erroneusnickname(const Client&, const std::string&);
		void send_err_nicknameinuse(const Client&, const std::string&);
		void send_err_nosuchchannel(const Client&, const std::string&);
		void send_err_notonchannel(const Client&, const std::string&);
		void send_err_badchannelkey(const Client&, const std::string&);
		void send_err_needmoreparams(const Client&, const std::string&);
		void send_err_alreadyregistered(const Client&);
		void send_err_norecipient(const Client&, const std::string&);
		void send_err_notexttosend(const Client&, const std::string&);
		void send_err_cannotsendtochan(const Client&, const std::string&);
		void send_err_notregistered(const Client&);
		void send_err_passwdmismatch(const Client&);
		void send_err_unknowncommand(const Client&, const std::string&);
		void send_err_chanoprivsneeded(const Client&, const std::string&);
		void send_err_unknownmode(const Client&, const char);
		void send_err_channelisfull(const Client&, const std::string&);

		std::map<std::string, Channel>& getChannels();
		std::map<SOCKET, Client>& getClients();
		Client& findClientByName(const std::string&);
		Client& getClientBySocket(SOCKET socket);
		std::vector<pollfd>& getPollfds();
		const std::string getPrefix() const;
		const std::string getIp() const;
		const std::string getPassword() const;
		const SOCKET getSocket() const;

		void setPassword(const std::string& password);
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

		// password
		std::string _password;

};

#endif // SERVER_HPP