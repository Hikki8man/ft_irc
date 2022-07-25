#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string>

#define SOCKET int

class Bot {
	public:
		Bot();
		Bot(const Bot&);
		Bot& operator=(const Bot&);
		Bot(const std::string& serverAddress, const int& port, const std::string& password);
		~Bot();

		int connectToServer();
		int run();
		void sendMessage(const std::string& message) const;
	private:
		std::string _serverAddress, _password, _nick, _user, _realName;
		struct sockaddr_in _addr;
		pollfd _pollfd;
		int _port;
};

#endif