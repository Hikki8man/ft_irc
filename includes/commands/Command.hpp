#ifndef IRC_COMMAND_HPP
#define IRC_COMMAND_HPP

#include "../Client.hpp"
#include "../common.hpp"
#include "../Irc.hpp"

class Client;
class CommandExecutor;

class Command {

	private:
		std::string					_name;
		std::vector<std::string>	_args;
		CommandExecutor *_executor;

	public:
		Command();
		Command(const std::string& toParse);
		Command(const std::string& name, const std::vector<std::string>& args);

		const std::string& getName() const;
		const std::vector<std::string>& getArgs() const;
		const Client& getSender() const;
		CommandExecutor *getExecutor() const;

		CommandExecutor *parse(std::string& buffer);
};

#endif