#include <iostream>

#include "../includes/server/Server.hpp"

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

	int 					server_fd;
	struct sockaddr_in		address;
	int 					opt = 1;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << std::strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		std::cerr << std::strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}


	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
		std::cerr << std::strerror(errno) << std::endl;
		return EXIT_FAILURE;
    }

	if (listen(server_fd, 3) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}