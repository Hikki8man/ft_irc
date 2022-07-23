#include "../includes/Server.hpp"

// ========== RPL_WELCOME (001) ==========
void Server::send_rpl_welcome(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg;
		msg = getPrefix() + " 001 " + client.getNickname() + " :Que des bandits ici ! Fais attention à toi " + client.getNickname() + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_WELCOME message to client" << std::endl;
	}
}

// ========== RPL_LISTSTART (321) ==========
void Server::send_rpl_liststart(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg;
		msg = getPrefix() + " 321 " + client.getNickname() + " Channel :Users  Name" + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_LISTSTART message to client" << std::endl;
	}
}

// ========== RPL_LIST (322) ==========
void Server::send_rpl_list(const Client& client, const Channel& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg;
		msg = getPrefix() + " 322 " + client.getNickname() + " " + channel.getName() + " " + intToString(channel.getClientsAndMode().size()) + " :" + channel.getTopic() + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_LIST message to client" << std::endl;
	}
}

// ========== RPL_LISTEND (323) ==========
void Server::send_rpl_listend(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg;
		msg = getPrefix() + " 323 " + client.getNickname() + " :End of /LIST" + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_LISTEND message to client" << std::endl;
	}
}

// ========== RPL_CHANNELMODEIS (324) ==========

void Server::send_rpl_channelmodeis(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string modesInfo = "";
		if (chan.hasMode(LIMIT))
			modesInfo += " " + std::to_string(chan.getLimit());
		if (chan.hasMode(KEY))
			modesInfo += " " + chan.getKey();
		std::string msg = getPrefix() + " 324 " + client.getNickname() + " " + chan.getName() + " +" + chan.getModes() + modesInfo + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_CHANNELMODEIS message to client" << std::endl;
	}
}

// ========== RPL_NOTOPIC (331) ==========
void Server::send_rpl_notopic(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 331 " + client.getNickname() + " " + chan.getName() + " :No topic is set" + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_NOTOPIC message to client" << std::endl;
	}
}

// ========== RPL_TOPIC (332) ==========
void Server::send_rpl_topic(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 332 " + client.getNickname() + " " + chan.getName() + " :" + chan.getTopic() + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_TOPIC message to client" << std::endl;
	}
}

// ========== RPL_TOPICWHOTIME (333) ==========
void Server::send_rpl_topicwhotime(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 333 " + client.getNickname() + " " + chan.getName() + " " + chan.getTopicCreatorAndWhen() + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_TOPICWHOTIME message to client" << std::endl;
	}
}

// ========== RPL_NAMREPLY (353) ==========
void  Server::send_rpl_namreply(const Client& client, const Channel& chan) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string prefix = getPrefix() + " 353 " + client.getNickname() + " = " + chan.getName() + " :";

		std::string clients_names;

		// Get all clients names
		for (std::map<SOCKET, char>::const_iterator it = chan.getClientsAndMode().begin(); it != chan.getClientsAndMode().end(); it++) {
			if (it->second != '\0')
				clients_names += it->second;
			clients_names += getClientBySocket(it->first).getNickname() + " ";
		}
		clients_names.erase(clients_names.size() - 1);
		int ret;
		// Parse the message and making sure it's not too long
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

// ========== RPL_ENDOFWHOIS (318) ==========

void Server::send_rpl_endofwhois(const Client& client, const std::string& name) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 318 " + client.getNickname() + " " + name + " :End of /WHOIS list." + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_ENDOFWHOIS message to client" << std::endl;
	}
}

// ========== RPL_WHOISUSER (311) ==========

void Server::send_rpl_whoisuser(const Client& client, const Client& user) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 311 " + client.getNickname() + " " + user.getNickname() + " " + user.getUsername() + " " + getIp() + " * :" + user.getRealName() + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_WHOISUSER message to client" << std::endl;
	}
}

// ========== RPL_WHOISHOST (378) ==========

void Server::send_rpl_whoishost(const Client& client, const Client& user) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 378 " + client.getNickname() + " " + user.getNickname() + " :is connecting from *@localhost 127.0.0.1" + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_WHOISHOST message to client" << std::endl;
	}
}

// ========== RPL_MOTDSTART (375) ==========

void Server::send_rpl_motdstart(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 375 " + client.getNickname() + " :- ft_irc Message of the day - " + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_MOTDSTART message to client" << std::endl;
	}
}

// ========== RPL_MOTD (372) ==========

void Server::send_rpl_motd(const Client& client, const std::string& motd) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 372 " + client.getNickname() + " :" + motd + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_MOTD message to client" << std::endl;
	}
}

// ========== RPL_ENDOFMOTD (376) ==========

void Server::send_rpl_endofmotd(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 376 " + client.getNickname() + " :End of /MOTD command." + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_ENDOFMOTD message to client" << std::endl;
	}
}

// ========== RPL_TIME (391) ==========

void Server::send_rpl_time(const Client& client, const std::string& time) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 391 " + client.getNickname() + " :" + time + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_TIME message to client" << std::endl;
	}
}

// ========== RPL_INVITING (341) ==========

void Server::send_rpl_inviting(const Client& client, const std::string& name, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 341 " + client.getNickname() + " " + name + " " + channel + CRLF;
		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cerr << "Error while sending RPL_INVITING message to client" << std::endl;
	}
}
