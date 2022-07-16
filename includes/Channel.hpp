#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include "Client.hpp"

class Channel {
	public:
		Channel();
		Channel(const std::string& name);
		Channel(const Channel&);
		Channel& operator=(const Channel&);
		~Channel();
		
		void setName(const std::string& name);
		void setKey(const std::string& key);

		const std::string getName() const;
		const std::string getKey() const;
		
		void addClient(const Client&);
		void removeClient(const Client&);
		
	private:
		std::string _name, _key;
		std::map<SOCKET, Client&> _clients;
};

#endif