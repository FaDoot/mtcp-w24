#include "socket/message_socket/message_socket.hpp"
#include <stdexcept>
#include <string>



MessageSocket::~MessageSocket() {
    close();
}

void MessageSocket::sendMessage(std::string message) {


    char buffer;
    ssize_t check = recv(file_descriptor_, &buffer, 1, MSG_PEEK | MSG_DONTWAIT);
    if (check == 0) {

        throw PeerClosedException();
    } else if (check < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        throw SocketException("Error checking connection state: " + std::string(strerror(errno)));
    }


    if (file_descriptor_ < 0) {
        throw SocketException("file descriptor error at send message");
    }
    if (message.find('\n') != std::string::npos) {
        throw SocketException("Message contains newline character");
}
    message += '\n';    

    for (const char& c : message) {
        ssize_t bytes_sent = send(file_descriptor_, &c, 1, 0);
        if (bytes_sent == -1) {
            throw SocketException("sending message failed " + std::string(strerror(errno)));
        }
        if (bytes_sent == 0) {
  
            throw SocketException("The socket was closed during sending");
        }
    }

}

std::string MessageSocket::receiveMessage() {
    std::string message;
    char ch;
    ssize_t bytes_received;

    while (true) {
        bytes_received = recv(file_descriptor_, &ch, 1, 0); 
        if (bytes_received == -1) {
            throw SocketException("Receiving message failed: " + std::string(strerror(errno)));
        } else if (bytes_received == 0) {
            throw PeerClosedException();
        } else {
            if (ch == '\n') {
                break; 
            }
            message += ch; 
        }
    }
    return message; 
}

void MessageSocket::close() {
    if (is_open_) {
        ::close(file_descriptor_);
        is_open_ = false;
    }
}
