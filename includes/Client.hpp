#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <iostream>

#define SOCKET int

class Client {
	public:
		Client();
		Client(int socket, struct sockaddr_in addr);
		Client(const Client&);
		Client& operator=(const Client&);
		~Client();

		int getSocket() const;
		struct sockaddr_in getAddr() const;
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getBuffer() const;

		void setNickname(const std::string& nick);
		void setUsername(const std::string& user);
		void setBuffer(const std::string& buffer);

	private:
		SOCKET _socket;
		struct sockaddr_in _addr;

		std::string _nick, _user, _realName, _bufferStocked;
};

#endif