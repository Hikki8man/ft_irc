#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include "Client.hpp"

class Client;

class Channel {
	public:

		enum Mode { limit, invite, key, moderated, secret, protected_topic, no_external_msg };

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
		const ClientAndMod &getClientAndMod(const std::string& nick) const;
		const std::string getModeString() const;
		std::vector<Mode> getModes() const;
		const std::string getModeId(Channel::Mode mode) const;
		const Channel::Mode getModeById(char id) const;
		
		void addClient(const Client&, const std::string& key = "");
		void removeClient(const Client&, const std::string& reason = "");
		
	private:
		std::string 				_name, _key;
		std::map<int, ClientAndMod>	_clients;
		std::vector<Mode>			_modes;
};

#endif