#ifndef IRC_COMMAND_HPP
#define IRC_COMMAND_HPP

#include "../Client.hpp"
#include "../common.hpp"

class Client;

class Command {

	private:
		std::string					_name;
		std::vector<std::string>	_args;
		Client&						_sender;

	public:
		Command(const std::string& name, const std::vector<std::string>& args, Client& sender);

		const std::string& getName() const;
		std::vector<std::string>& getArgs();
		const Client& getSender() const;
};

#endif