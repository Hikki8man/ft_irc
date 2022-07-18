#include "../includes/Server.hpp"

// ========== ERR_NOSUCHCHANNEL (403) ==========
void Server::send_err_nosuchchannel(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 403 " + client.getNickname() + " " + channel + " :No such channel" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NONICKNAMEGIVEN (431) ==========
void Server::send_err_nonicknamegiven(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string nick = client.getNickname();
		if (nick.empty())
			nick = "*";
		std::string msg = getPrefix() + " 431 " + nick + " :No nickname given" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_ERRONEUSNICKNAME (432) ==========
void Server::send_err_erroneusnickname(const Client& client, const std::string& nickname) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string nick = client.getNickname();
		if (nick.empty())
			nick = "*";
		std::string msg = getPrefix() + " 432 " + nick + " " + nickname + " :Erroneous nickname" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NICKNAMEINUSE (433) ==========
void Server::send_err_nicknameinuse(const Client& client, const std::string& nickname) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string nick = client.getNickname();
		if (nick.empty())
			nick = "*";
		std::string msg = getPrefix() + " 433 " + nick + " " + nickname + " :Nickname is already in use" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NOTONCHANNEL (442) ==========
void Server::send_err_notonchannel(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 442 " + client.getNickname() + " " + channel + " :You're not on that channel" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NEEDMOREPARAMS (461) ==========
void Server::send_err_needmoreparams(const Client& client, const std::string& command) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 461 " + client.getNickname() + " " + command + " :Not enough parameters" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_ALREADYREGISTRED (462) ==========
void Server::send_err_alreadyregistered(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 462 " + client.getNickname() + " :You may not reregister" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}