#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include "Client.hpp"

#define SECRET 's'
#define PROTECTED_TOPIC 't'
#define NO_EXTERNAL_MESSAGES 'n'
#define INVITE 'i'
#define KEY 'k'
#define LIMIT 'l'
#define MODERATED 'm'

class Client;

class Channel {
	public:

		struct ClientAndMod {
			ClientAndMod(const Client&, char);

			const Client &client;
			char mod;
		};
		
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
		const std::map<int, ClientAndMod>& getClients() const;
		const Client &findClientByName(const std::string& nick) const;
		const ClientAndMod &getClientAndMod(const std::string& nick) const;
		std::string getModes() const;
		
		void addClient(const Client&, const std::string& key = "");
		void removeClient(const Client&, const std::string& reason = "");

		void addMode(char mode);
		void removeMode(char mode);
		const bool hasMode(char mode) const;
		
	private:
		std::string 				_name, _key;
		std::map<int, ClientAndMod>	_clients;
		std::string					_modes;
		int							_limit;
};

#endif