#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <iostream>
#include <poll.h>
#include <map>
#include <vector>
#include "Irc.hpp"

#define SOCKET int

#define INVISIBLE 'i'
#define OPERATOR 'o'
#define VOICE 'v'

class Channel;

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
		const pollfd& getPollfd() const;
		std::string& getBuffer();
		const std::string getPrefix() const;
		const std::string getIp() const;
		std::map<std::string, const Channel&>& getChannels();

		void setNickname(const std::string& nick);
		void setUsername(const std::string& user);
		void setBuffer(const std::string& buffer);
		void setRealName(const std::string& realName);
		void setPrefix();
		void setRegistered(bool);
		void setLogged(bool);
		void setIp(const std::string& ip);

		void addChannel(const Channel& channel);
		void removeChannel(Channel& channel);
		bool isRegistered() const;
		bool isLogged() const;
		bool isInChannel(const Channel& channel) const;

		void sendMessage(int toSend, const std::string& message, bool prefix = true) const;
		void sendMessage(Client &toSend, const std::string& message, bool prefix = true) const;

	private:
		bool _registered;
		bool _logged;
		SOCKET _socket;
		struct sockaddr_in _addr;

		std::map<std::string, const Channel&> _channels;
		std::string _nick, _user, _realName, _bufferStocked, _prefix, _ip;
};

#endif