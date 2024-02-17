#include "socket/message_socket/client_message_socket.hpp"
#include <iostream>
ClientMessageSocket::ClientMessageSocket(int domain, const std::string& server_address, int port)
    : ClientSocket(domain, server_address, port), MessageSocket() {
    // The actual socket setup occurs in the ClientSocket constructor.
    if(file_descriptor_ < 0){
         std::cout << "ERROR WITH FD ON creation \n";
    }
    // Initialization for MessageSocket part if needed.
}

// No additional definition is required for the destructor, as we use the default one.