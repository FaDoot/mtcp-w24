#include "socket/message_socket/connection_message_socket.hpp"
#include <iostream>


ConnectionMessageSocket::ConnectionMessageSocket(int file_descriptor)
    : ConnectionSocket(file_descriptor), MessageSocket(), Socket(file_descriptor) {
    if (file_descriptor < 0) {
        throw std::invalid_argument("ConnectionMessageSocket constructed with an invalid file descriptor");
    
    }
   
}



