#include "Bot.hpp"

int main(int ac, char **av) {
	srand(time(NULL));
	if (ac < 3 || ac > 4) {
		std::cerr << "Usage: " << av[0] << " <server address> <port> <password>" << std::endl;
		return EXIT_FAILURE;
	}
	std::string password;
	int port = atoi(av[2]);
	if (ac == 4)
		password = av[3];
	Bot bot(av[1], port, password);
	bot.connectToServer();
	bot.run();

	return 0;
}