#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <csignal>

int main() {
    int status;
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
    int fd = socket(servinfo->ai_family, servinfo->ai_socktype,
                    servinfo->ai_protocol);
    if (fd == -1) {
        std::cout << "Could not get file descriptor for address info.\n"
                  << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        std::cout << "Could not bind to network file descriptor.\n"
                  << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Hello, World!" << std::endl;
    
    freeaddrinfo(servinfo);
    if (close(fd) == -1) {
        std::cout << "Could not close file descriptor.\n" << strerror(errno)
                  << std::endl;
        exit(EXIT_FAILURE);
    };
    return 0;
}
