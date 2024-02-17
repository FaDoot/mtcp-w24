#include "socket/client_socket.hpp"
#include <stdexcept> // for std::runtime_error
#include <cstring> // for memset
#include <iostream>

ClientSocket::ClientSocket(int domain, const std::string& server_address, int port) {
    file_descriptor_ = socket(domain, SOCK_STREAM, 0);
    if (file_descriptor_ == -1) {
        throw SocketException("Socket creation failed");
    }

    memset(&server_address_, 0, sizeof(server_address_));
    server_address_.sin_family = domain;
    server_address_.sin_port = htons(port);

    if (inet_pton(domain, server_address.c_str(), &server_address_.sin_addr) <= 0) {
        // Close the file descriptor before throwing an exception to prevent resource leakage
        ::close(file_descriptor_);
        throw SocketException("Invalid server address format");
    }
}

ClientSocket::~ClientSocket() {
    // Even though the destructor is pure virtual, we need to provide an implementation for it
    close(file_descriptor_); // Clean up the file descriptor if it is still open.
     std::cout << "is it happening at close" <<std::endl;
}

void ClientSocket::connect() {
    if (file_descriptor_ < 0) {
        throw SocketException("Invalid file descriptor with connect ");
    }
    if (::connect(file_descriptor_, (struct sockaddr *)&server_address_, sizeof(server_address_)) < 0) {
        // Close the socket if the connection attempt fails.
        ::close(file_descriptor_);
        file_descriptor_ = -1;
        std::cout << "is it happening at connect" <<std::endl;
        throw SocketException("Connection to server failed");
    }
}
