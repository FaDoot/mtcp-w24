#include "socket/message_socket/connection_message_socket.hpp"
#include <iostream>


ConnectionMessageSocket::ConnectionMessageSocket(int file_descriptor)
    : ConnectionSocket(file_descriptor), MessageSocket() {
    if (file_descriptor < 0) {
        throw std::invalid_argument("ConnectionMessageSocket constructed with an invalid file descriptor");
    
    }
    //file_descriptor_ = file_descriptor;
    // Additional initializations.
    std::cout << file_descriptor<<"works at the constructor level \n";
}

// No additional definition is required for the destructor, as we use the default one.


