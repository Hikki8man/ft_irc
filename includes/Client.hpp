#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <iostream>
#include <poll.h>

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
		const std::string getNickname() const;
		const std::string getUsername() const;
		const std::string getRealName() const;
		const pollfd* getPollfd() const;
		std::string& getBuffer();

		void setNickname(const std::string& nick);
		void setUsername(const std::string& user);
		void setBuffer(const std::string& buffer);
		void setRealName(const std::string& realName);
		void setPollfd(const pollfd& pollfd);
		void setRegistered(bool);

		bool isRegistered() const;

	private:
		bool _registered;
		SOCKET _socket;
		const pollfd *_pollfd;//pas bon
		struct sockaddr_in _addr;

		std::string _nick, _user, _realName, _bufferStocked;
};

#endif