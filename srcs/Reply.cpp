#include "../includes/Server.hpp"

// ========== RPL_WELCOME (001) ==========
void Server::send_rpl_welcome(const Client& client) {

	if (client.getPollfd().revents & POLLOUT) {
		std::string msg;
		msg = getPrefix() + " 001 " + client.getNickname() + " :Que des bandits ici ! Fait attention a toi " + client.getNickname() + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_WELCOME message to client" << std::endl;
	}
}

// ========== JOIN ==========
void  Server::send_join(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
			std::string msg = client.getPrefix() + " JOIN " + chan.getName() + CRLF;
			int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
			if (ret == -1)
				std::cerr << "Error while sending JOIN message to client" << std::endl;			
		}
}

// ========== RPL_NAMREPLY (353) ==========
void  Server::send_rpl_namreply(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string prefix = getPrefix() + " 353 " + client.getNickname() + " = " + chan.getName() + " :";

		std::string clients_names;

		// Get all clients names
		for (std::map<int, Channel::ClientAndMod>::const_iterator it = chan.getClients().begin(); it != chan.getClients().end(); it++) {
			if (it->second.mod != '\0')
				clients_names += it->second.mod;
			clients_names += it->second.client.getNickname() + " ";
		}
		clients_names.erase(clients_names.size() - 1);
		int ret;
		while (!clients_names.empty()) {
			std::string msg = prefix;
			if (clients_names.size() + msg.length() > BUFFER_MAX + 2) {
				msg += clients_names.substr(0, clients_names.find_last_of(" ", BUFFER_MAX - prefix.size() - 2) - 1);
				clients_names.erase(0, clients_names.find_last_of(" ", BUFFER_MAX - prefix.size() - 2));
				msg += CRLF;
				ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
				msg.clear();
				if (ret == -1)
					break;
			}
			else {
				msg += clients_names + CRLF;
				ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
				clients_names.clear();
			}
		}
		if (ret == -1)
			std::cerr << "Error while sending RPL_NAMREPLY message to client" << std::endl;
	}
}

// ========== RPL_ENDOFNAMES (366) ==========
void Server::send_rpl_endofnames(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 366 " + client.getNickname() + " " + chan.getName() + " :End of /NAMES list." + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_ENDOFNAMES message to client" << std::endl;
	}
}