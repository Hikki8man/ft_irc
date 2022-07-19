#include "commands/CommandManager.hpp"

#include "commands/impl/NickCommand.hpp"
#include "commands/impl/UserCommand.hpp"
#include "commands/impl/JoinCommand.hpp"
#include "commands/impl/PartCommand.hpp"
#include "commands/impl/PrivmsgCommand.hpp"
#include "commands/impl/PingCommand.hpp"
#include "commands/impl/PassCommand.hpp"

CommandManager::CommandManager()
{
	addCommand("nick", new NickCommand());
	addCommand("user", new UserCommand());
	addCommand("join", new JoinCommand());
	addCommand("part", new PartCommand());
	addCommand("privmsg", new PrivmsgCommand());
	addCommand("ping", new PingCommand());
	addCommand("pass", new PassCommand());
}

void CommandManager::addCommand(const std::string& name, CommandExecutor* command)
{
	_commands.insert(std::pair<std::string, CommandExecutor*>(name, command));
}

CommandExecutor *CommandManager::getCommand(const std::string& name)
{
	// TODO or not: The command must either be a valid IRC command or a numeric
	if (_commands.find(name) != _commands.end())
		return _commands[name];
	return NULL;
}

std::map<std::string, CommandExecutor*> CommandManager::getCommands()
{
	return _commands;
}

CommandManager::~CommandManager()
{
	for (std::map<std::string, CommandExecutor*>::iterator it = _commands.begin(); it != _commands.end(); ++it)
		delete it->second;
}