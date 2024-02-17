#include "socket/message_socket/client_message_socket.hpp"
#include <iostream>
ClientMessageSocket::ClientMessageSocket(int domain, const std::string& server_address, int port)
    : ClientSocket(domain, server_address, port), MessageSocket(), Socket(domain) {

    if(file_descriptor_ < 0){
    }

}

