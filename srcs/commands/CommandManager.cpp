#include "commands/CommandManager.hpp"

#include "commands/impl/NickCommand.hpp"
#include "commands/impl/UserCommand.hpp"
#include "commands/impl/JoinCommand.hpp"
#include "commands/impl/PartCommand.hpp"
#include "commands/impl/PrivmsgCommand.hpp"
#include "commands/impl/PingCommand.hpp"
#include "commands/impl/PassCommand.hpp"
#include "commands/impl/NoticeCommand.hpp"
#include "commands/impl/ModeCommand.hpp"
#include "commands/impl/QuitCommand.hpp"
#include "commands/impl/NamesCommand.hpp"
#include "commands/impl/WhoisCommand.hpp"
#include "commands/impl/MotdCommand.hpp"
#include "commands/impl/TimeCommand.hpp"
#include "commands/impl/InviteCommand.hpp"
#include "commands/impl/KickCommand.hpp"

CommandManager::CommandManager()
{
	addCommand("NICK", new NickCommand());
	addCommand("USER", new UserCommand());
	addCommand("JOIN", new JoinCommand());
	addCommand("PART", new PartCommand());
	addCommand("PRIVMSG", new PrivmsgCommand());
	addCommand("PING", new PingCommand());
	addCommand("PASS", new PassCommand());
	addCommand("NOTICE", new NoticeCommand());
	addCommand("MODE", new ModeCommand());
	addCommand("QUIT", new QuitCommand());
	addCommand("NAMES", new NamesCommand());
	addCommand("WHOIS", new WhoisCommand());
	addCommand("MOTD", new MotdCommand());
	addCommand("TIME", new TimeCommand());
	addCommand("INVITE", new InviteCommand());
	addCommand("KICK", new KickCommand());
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