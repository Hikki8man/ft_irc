#include "commands/CommandManager.hpp"

#include "commands/impl/NickCommand.hpp"
#include "commands/impl/UserCommand.hpp"

CommandManager::CommandManager()
{
	addCommand("nick", new NickCommand());
	addCommand("user", new UserCommand());
}

void CommandManager::addCommand(const std::string& name, CommandExecutor* command)
{
	_commands.insert(std::pair<std::string, CommandExecutor*>(name, command));
}

CommandExecutor *CommandManager::getCommand(const std::string& name)
{
	if (_commands.find(name) != _commands.end())
		return _commands[name];
	return nullptr;
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