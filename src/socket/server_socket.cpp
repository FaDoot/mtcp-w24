#include "socket/server_socket.hpp"
#include "socket/connection_socket.hpp"
#include <iostream> 
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <cstring>


// Constructor for server socket with IP and port
//makes socket with domain which is like 2 or something, arrd = "127.0.0.1" I THINK, port 9999, backlog is num listeenrs
ServerSocket::ServerSocket(int const domain, const std::string &address, int port, int const listen_backlog) {
    // Create a socket
    file_descriptor_ = socket(domain, SOCK_STREAM, 0);
    if (file_descriptor_ == -1) {
        // You should handle the error with proper exception handling or error reporting mechanism
        throw std::runtime_error("Failed to create socket");
    }

    // Zero the structure
    memset(&server_address_, 0, sizeof(server_address_));

    server_address_.sin_family = domain; // This should typically be AF_INET for IPv4
    server_address_.sin_port = htons(port); // Make sure to use network byte order

    // Convert the IP address string to a network address
    if (inet_pton(domain, address.c_str(), &server_address_.sin_addr) <= 0) {
        // Close the socket file descriptor before throwing
        ::close(file_descriptor_);
        throw std::runtime_error("Invalid IP address format");
    }

    // Bind the socket
    int optval = 1;
    if (setsockopt(file_descriptor_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        ::close(file_descriptor_);
        throw std::runtime_error("ServerSocket: Failed to set SO_REUSEADDR");
}
    if (::bind(file_descriptor_, (struct sockaddr*)&server_address_, sizeof(server_address_)) < 0) {
        // Close the socket file descriptor before throwing
        ::close(file_descriptor_);
        throw std::runtime_error("Failed to bind");
    }

    // Listen to incoming connections
    if (listen(file_descriptor_, listen_backlog) < 0) {
        // Close the socket file descriptor before throwing
        ::close(file_descriptor_);
        throw std::runtime_error("Failed to listen on socket");
    }
    is_open_ = true;
    std::cout << "FDvalue" << file_descriptor_ << std::endl;
}
// Constructor for server socket which determines domain/address automatically
ServerSocket::ServerSocket(int domain, int listen_backlog) {
    file_descriptor_ = socket(domain, SOCK_STREAM, 0);
    if (file_descriptor_ == -1) {
        throw std::runtime_error("ServerSocket: Failed to create socket");
    }

    memset(&server_address_, 0, sizeof(server_address_));

    server_address_.sin_family = AF_INET;
    server_address_.sin_port = 0; // 0 means the system will automatically assign an available port
    server_address_.sin_addr.s_addr = INADDR_ANY; // Listen on any interface
    int optval = 1;
if (setsockopt(file_descriptor_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    ::close(file_descriptor_);
    throw std::runtime_error("ServerSocket: Failed to set SO_REUSEADDR");
}

    if (::bind(file_descriptor_, (struct sockaddr*)&server_address_, sizeof(server_address_)) < 0) {
        ::close(file_descriptor_);
        throw std::runtime_error("ServerSocket: Failed to bind");
    }

    if (listen(file_descriptor_, listen_backlog) < 0) {
        ::close(file_descriptor_);
        throw std::runtime_error("ServerSocket: Failed to listen on socket");
    }

    is_open_ = true; // Mark the socket as open
}

ServerSocket::~ServerSocket() {
    // Close socket if open
    this->close();
}

// Socket::~Socket() {
//     if (file_descriptor_ >= 0) {
//         ::close(file_descriptor_);
//     }
// }

std::pair<std::string, int> ServerSocket::getAddressInfo(int domain) {
    // Your implementation assumes that you will always get an IP and port for a given domain.
    // Typically, domain would be AF_INET or AF_INET6 and this function would return an
    // IP/port pair appropriate for that domain.

    // Since domain typically does not directly map to a specific IP address and port number
    // without additional context (like if you're on a server or client), the implementation below is 
    // a placeholder and will need to be fleshed out based on the precise needs of your application.

    switch (domain) {
        case AF_INET: {
            // Example for IPv4
            return std::make_pair("0.0.0.0", 0); // This represents INADDR_ANY and an arbitrary port
        }
        case AF_INET6: {
            // Example for IPv6
            return std::make_pair("::", 0); // This represents in6addr_any and an arbitrary port
        }
        default:
            throw std::invalid_argument("Unsupported domain");
    }
}

// acceptConnection<T> is templated and most likely defined in the header due to necessity of implementation being visible

