#ifndef COMMANDCODE_HPP
#define COMMANDCODE_HPP

#include <iostream>

enum CommandCode {
	NICK,
	USER,
	JOIN,
	PART,
	QUIT,
	PRIVMSG,
	PING,
	PONG,
	UNKNOWN
};

#endif