#ifndef IRC_COMMAND_MANAGER_HPP
#define IRC_COMMAND_MANAGER_HPP

#include "CommandExecutor.hpp"
#include <map>

class CommandExecutor;

class CommandManager
{
	private:
		std::map<std::string, CommandExecutor*> _commands;
	public:
		CommandManager();
		~CommandManager();

		void addCommand(const std::string& name, CommandExecutor* command);
		CommandExecutor *getCommand(const std::string& name);
		std::map<std::string, CommandExecutor*> getCommands();
};



#endif