#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include <ctime>
#include "Client.hpp"

#define SECRET 's' // TODO: handle properly
#define PROTECTED_TOPIC 't'
#define NO_EXTERNAL_MESSAGES 'n'
#define INVITE_ONLY 'i'
#define KEY 'k'
#define LIMIT 'l'
#define MODERATED 'm'

#define OP '@'
#define NONE '\0'

#define SOCKET int

class Client;

class Channel {
	public:
		
		Channel();
		Channel(const std::string& name);
		Channel(const Channel&);
		Channel& operator=(const Channel&);
		~Channel();
		
		void setName(const std::string& name);
		void setKey(const std::string& key);
		void setLimit(const int limit);
		void setTopic(const Client& client, const std::string& topic);

		const std::string getName() const;
		const std::string getKey() const;
		const int getLimit() const;
		const std::map<SOCKET, char>& getClientsAndMode() const;
		char getClientMode(const SOCKET&) const;
		const std::string getTopic() const;
		const std::string getTopicCreatorAndWhen() const;
		std::string getModes() const;
		
		void clearTopic();
		void addClient(Client&, const std::string& key = "");
		void insertClient(std::pair<SOCKET, char>);
		void removeClient(const Client&, const std::string& cmd, const std::string& reason = "");
		void addInvite(const Client&);
		void removeInvite(const Client&);
		const bool isInvited(const Client&) const;

		void addMode(char mode);
		void removeMode(char mode);
		const bool hasMode(char mode) const;
		
	private:

		void setTopicCreatorAndWhen(const Client& client);

		std::string 				_name, _key;
		std::map<SOCKET, char>		_clientsAndMode;
		std::vector<SOCKET>			_invites;
		std::string					_modes;
		std::string					_topic, _topicCreatorAndWhen;
		int							_limit;
};

#endif