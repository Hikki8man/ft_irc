#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include "Client.hpp"

#define SECRET 's'
#define PROTECTED_TOPIC 't'
#define NO_EXTERNAL_MESSAGES 'n'
#define INVITE_ONLY 'i'
#define KEY 'k'
#define LIMIT 'l'
#define MODERATED 'm'

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

		const std::string getName() const;
		const std::string getKey() const;
		const int getLimit() const;
		std::map<SOCKET, char>& getClientsAndMod();
		std::string getModes() const;
		
		void addClient(Client&, const std::string& key = "");
		void removePartClient(const Client&, const std::string& reason = "");
		void removeQuitClient(const Client&);

		void addInvite(const Client&);
		void removeInvite(const Client&);
		const bool isInvited(const Client&) const;

		void addMode(char mode);
		void removeMode(char mode);
		const bool hasMode(char mode) const;
		
	private:
		std::string 				_name, _key;
		std::map<SOCKET, char>		_clientsAndMod;
		std::vector<SOCKET>			_invites;
		std::string					_modes;
		int							_limit;
};

#endif