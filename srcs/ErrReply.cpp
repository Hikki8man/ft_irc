#include "../includes/Server.hpp"

// ========== ERR_NOSUCHCHANNEL (403) ==========
void Server::send_err_nosuchchannel(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 403 " + client.getNickname() + " " + channel + " :No such channel" + CRLF;
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