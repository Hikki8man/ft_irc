#ifndef IRC_HPP
#define IRC_HPP

#include "Server.hpp"
#include "commands/CommandManager.hpp"

class Server;
class CommandManager;

class Irc
{
	private:
		static Irc 		*instance;
		Server 			*server;
		CommandManager	commandManager;
	public:
		static Irc& getInstance();

		Irc();

		Server*		getServer();
		void	 	setServer(Server* server);

		CommandManager& 	getCommandManager();
		void				setCommandManager(CommandManager& commandManager);
};




#endif