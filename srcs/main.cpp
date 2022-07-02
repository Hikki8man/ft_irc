#include <iostream>

int main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return EXIT_FAILURE;
	}

	int port = atoi(av[1]);

	if (port < 1024 || port > 65535) { // https://docstore.mik.ua/orelly/networking_2ndEd/fire/ch19_01.htm#:~:text=IRC%20is%20a%20TCP%2Dbased,to%20other%20clients%20using%20DCC.
		std::cerr << "Invalid port: " << port << std::endl;
		return EXIT_FAILURE;
	}
	
	std::cout << "port: " << port << std::endl;
}