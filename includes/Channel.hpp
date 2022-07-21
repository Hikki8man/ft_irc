#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include "Client.hpp"

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

		const std::string getName() const;
		const std::string getKey() const;
		const std::map<int, ClientAndMod>& getClients() const;
		const Client &findClientByName(const std::string& nick) const;
		
		void addClient(const Client&, const std::string& key = "");
		void removeClient(const Client&, const std::string& reason = "");
		
	private:
		std::string _name, _key;
		std::map<int, ClientAndMod> _clients;
};

#endif