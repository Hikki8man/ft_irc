#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <poll.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "new connection" << std::endl;
    pollfd fds;
    fds.fd = new_socket;
    fds.events = POLLIN | POLLOUT;
    // fds.revents = 0;
    int i = 0;
    bool sended = false;
    while (1) {
        int ret = poll(&fds, 1, 0);
        if (ret == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        } else if (ret > 0) {
            // std::cout << "poll" << std::endl;
            if (fds.revents & POLLERR) {
                std::cout << "POLLERR" << std::endl;
                break;
            }
            else if (fds.revents & (POLLHUP | POLLNVAL)) {
                std::cout << "POLLHUP | POLLNVAL" << std::endl;
                break;
            }
            else if (fds.revents & POLLIN) {
                valread = read(new_socket, buffer, 1024);
                std::cout << "valread: " << valread << std::endl;
                if (valread == 0) {
                    std::cout << "client disconnected" << std::endl;
                    break;
                }
                std::cout << "received: " << buffer << std::endl;
            }
            else if (fds.revents & POLLOUT && !sended) {
                std::string str = "Hello from server fewfwefewfwe\n";
                if (send(new_socket, str.c_str(), str.length(), 0) < 0) {
                    perror("send");
                    exit(EXIT_FAILURE);
                }
                std::cout << "sended" << std::endl;
                sended = true;
            }
            // if (i == 120)
            //     break;
        }
        ++i;
    }

    return 0;
}