#include "../includes/Server.hpp"

// ========== ERR_NOSUCHNICK ==========
void Server::send_err_nosuchnick(const Client& client, const std::string& nick) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 401 " + client.getNickname() + " " + nick + " :No such nick" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NOSUCHCHANNEL (403) ==========
void Server::send_err_nosuchchannel(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 403 " + client.getNickname() + " " + channel + " :No such channel" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_CANNOTSENDTOCHAN (404) ==========
void Server::send_err_cannotsendtochan(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 404 " + client.getNickname() + " " + channel + " :Cannot send to channel" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NORECIPIENT (411) ==========
void Server::send_err_norecipient(const Client& client, const std::string& command) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 411 " + client.getNickname() + " " + command + " :No recipient given" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NOTEXTTOSEND (412) ==========
void Server::send_err_notexttosend(const Client& client, const std::string& command) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 412 " + client.getNickname() + " :No text to send " + "(" + command + ")" + CRLF;
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

// ========== ERR_BADCHANNELKEY (475) ==========
void Server::send_err_badchannelkey(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 475 " + client.getNickname() + " " + channel + " :Cannot join channel (+k)" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NOTREGISTERED (451) ==========

void Server::send_err_notregistered(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 451 " + client.getNickname() + " :You have not registered" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_PASSWDMISMATCH (464) ==========

void Server::send_err_passwdmismatch(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 464 " + client.getNickname() + " :Password incorrect" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_UNKNOWNCOMMAND (421) ==========

void Server::send_err_unknowncommand(const Client& client, const std::string& command) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 421 " + client.getNickname() + " " + command + " :Unknown command" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_CHANOPRIVSNEEDED (482) ==========

void Server::send_err_chanoprivsneeded(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 482 " + client.getNickname() + " " + channel + " :You're not channel operator" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_UNKNOWNMODE (472) ==========

void Server::send_err_unknownmode(const Client& client, const char mode) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 472 " + client.getNickname() + " " + mode + " :is unknown mode char to me for that channel" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_CHANNELISFULL (471) ==========

void Server::send_err_channelisfull(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 471 " + client.getNickname() + " " + channel + " :Cannot join channel (+l) - channel is full" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_INVITEONLYCHAN (473) ==========

void Server::send_err_inviteonlychan(const Client& client, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 473 " + client.getNickname() + " " + channel + " :Cannot join channel (+i) - you must be invited" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_USERNOTINCHANNEL (442) ==========

void Server::send_err_usernotinchannel(const Client& client, const std::string& nick, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 442 " + client.getNickname() + " " + nick + " " + channel + " :They aren't on that channel" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_USERONCHANNEL (443) ==========
void Server::send_err_useronchannel(const Client& client, const std::string& nick, const std::string& channel) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 443 " + client.getNickname() + " " + nick + " " + channel + " :is already on channel" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}

// ========== ERR_NOPRIVILEGES (481) ==========

void Server::send_err_noprivileges(const Client& client) {
	if (client.getPollfd().revents & POLLOUT) {
		std::string msg = getPrefix() + " 481 " + client.getNickname() + " :Permission Denied - You're not an IRC operator" + CRLF;
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
	}
}