#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <csignal>

#define BACKLOG 5

int main() {
    int status;
    int net_fd, new_fd;
    struct sockaddr_storage their_addr{0};
    socklen_t addr_size = sizeof their_addr;
    struct sockaddr_in sa{};
    struct addrinfo hints{0};
    struct addrinfo *servinfo;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    errno = 0;
    if (inet_pton(AF_INET, "127.0.0.1", &(sa.sin_addr)) <= 0) {
        std::cout << "Something went wrong with translating the IP Address.\n"
                  << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if ((status = getaddrinfo(NULL, "8080", &hints, &servinfo)) != 0) {
        std::cout << "Could not getaddrinfo\n" << gai_strerror(status)
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    net_fd = socket(servinfo->ai_family, servinfo->ai_socktype,
                    servinfo->ai_protocol);
    if (net_fd == -1) {
        std::cout << "Could not get file descriptor for address info.\n"
                  << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(net_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        std::cout << "Could not bind to network file descriptor.\n"
                  << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(net_fd, BACKLOG) == -1) {
        std::cout << "Could not listen to net_fd.\n" << strerror(errno)
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    new_fd = accept(net_fd, (sockaddr *) (&their_addr), &addr_size);
    if (new_fd == -1) {
        std::cout << "Error accepting connection.\n" << strerror(errno)
                  << std::endl;
    }

    std::string msg = "Hello, World!";
    int msg_len, bytes_sent;

    msg_len = msg.length();
    bytes_sent = send(new_fd, msg.c_str(), msg_len, 0);

    if (bytes_sent == -1) {
        std::cout << "Error sending message.\n" << strerror(errno) << std::endl;
    } else {
        std::cout << "Sent Message!" << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;

    freeaddrinfo(servinfo);
    if (close(net_fd) == -1) {
        std::cout << "Could not close file descriptor.\n" << strerror(errno)
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
