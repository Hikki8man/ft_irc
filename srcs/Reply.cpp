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

// // ========== JOIN ==========
// void  Server::send_join(SOCKET _sender, SOCKET _joiner, const Channel& chan) {
// 	Client& sender = getClientBySocket(_sender);
// 	Client& joiner = getClientBySocket(_joiner);
// 	if (sender.getPollfd().revents & POLLOUT) {
// 			std::string msg = joiner.getPrefix() + " JOIN " + chan.getName() + CRLF;
// 			int ret = send(_sender, msg.c_str(), msg.size(), 0);
// 			if (ret == -1)
// 				std::cerr << "Error while sending JOIN message to client" << std::endl;			
// 		}
// }

// // ========== PART ==========
// void Server::send_part(const Client& client, const Client& leaver, const Channel& chan, const std::string& reason) {
// 	if (client.getPollfd().revents & POLLOUT) {
// 		std::string msg = leaver.getPrefix() + " PART " + chan.getName();
// 		if (!reason.empty())
// 			msg += " :" + reason;
// 		msg += CRLF;
// 		int ret = send(client.getSocket(), msg.c_str(), msg.size(), 0);
// 		if (ret == -1)
// 			std::cerr << "Error while sending PART message to client" << std::endl;
// 	}
// }

// // ========== QUIT ==========
// void Server::send_quit(const Client& client, SOCKET rcv, const std::string& reason) {
// 	Client& receiver = getClientBySocket(rcv);
// 	if (receiver.getPollfd().revents & POLLOUT) {
// 		std::string msg = client.getPrefix() + " QUIT :" + reason + CRLF;
// 		int ret = send(rcv, msg.c_str(), msg.size(), 0);
// 		if (ret == -1)
// 			std::cerr << "Error while sending QUIT message to client" << std::endl;
// 	}
// }

// // ========= PRIVMSG =========
// void Server::send_privmsg(const Client& sender, const Client& receiver, const std::string& name, const std::string& msg) {
// 	if (receiver.getPollfd().revents & POLLOUT) {
// 		std::string msg_to_send = sender.getPrefix() + " PRIVMSG " + name + " :" + msg + CRLF;
// 		int ret = send(receiver.getSocket(), msg_to_send.c_str(), msg_to_send.size(), 0);
// 		if (ret == -1)
// 			std::cerr << "Error while sending PRIVMSG message to client" << std::endl;
// 	}
// }

// // ========== NOTICE ==========
// void Server::send_notice(const std::string& prefix, const Client& receiver, const std::string& name, const std::string& msg) {
// 	if (receiver.getPollfd().revents & POLLOUT) {
// 		std::string n(name);
// 		if (name.empty())
// 			n = "*";
// 		std::string msg_to_send = prefix + " NOTICE " + name + " :" + msg + CRLF;
// 		int ret = send(receiver.getSocket(), msg_to_send.c_str(), msg_to_send.size(), 0);
// 		if (ret == -1)
// 			std::cerr << "Error while sending NOTICE message to client" << std::endl;
// 	}
// }

// // ========== ERROR ==========
// void Server::send_error(const Client& client, const std::string& msg) {
// 	if (client.getPollfd().revents & POLLOUT) {
// 		std::string msg_to_send = "ERROR :Closing Link: " + client.getIp() + " (" + msg + ")" + CRLF;
// 		int ret = send(client.getSocket(), msg_to_send.c_str(), msg_to_send.size(), 0);
// 		if (ret == -1)
// 			std::cerr << "Error while sending ERROR message to client" << std::endl;
// 		close(client.getSocket());
// 	}
// }

// ========== RPL_NAMREPLY (353) ==========
void  Server::send_rpl_namreply(const Client& client, const Channel& chann) {
	if (client.getPollfd().revents & POLLOUT) {
		Channel chan(chann); //TODO: temp find another way
		std::string prefix = getPrefix() + " 353 " + client.getNickname() + " = " + chan.getName() + " :";

		std::string clients_names;

		// Get all clients names
		for (std::map<SOCKET, char>::iterator it = chan.getClientsAndMod().begin(); it != chan.getClientsAndMod().end(); it++) {
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
		// TODO add real hostname
		std::string msg = getPrefix() + " 311 " + client.getNickname() + " " + user.getNickname() + " " + user.getUsername() + " localhost * :" + user.getRealName() + CRLF;
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