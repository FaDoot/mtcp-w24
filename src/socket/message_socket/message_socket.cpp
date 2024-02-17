#include "socket/message_socket/message_socket.hpp"
// Make sure to add necessary includes if you have more dependencies

MessageSocket::~MessageSocket() {
    // Make sure to close the socket if it is not already closed.
    close();
}

void MessageSocket::sendMessage(std::string message) {
    // Check for the delimiter character in the message, and throw if found.

    // Check the validity of the file descriptor.
    if (file_descriptor_ < 0) {
        throw SocketException("Invalid file descriptor at sendmessage");
    }

    // Loop to send the entire message, one character at a time.
    for (const char& c : message) {
        ssize_t bytes_sent = send(file_descriptor_, &c, 1, 0);
        if (bytes_sent == -1) {
            throw SocketException("Sending message failed: " + std::string(strerror(errno)));
        }
        if (bytes_sent == 0) {
            // The socket was closed by the peer or shutdown for writing.
            throw SocketException("The socket was closed during sending");
        }
    }
    // After sending the message, you would also need to send your delimiter.
}

std::string MessageSocket::receiveMessage() {
    std::string message;
    char ch;
    ssize_t bytes_received;

    // Loop to receive characters from the socket until the delimiter is found
    while (true) {
        bytes_received = recv(file_descriptor_, &ch, 1, 0); // Receive one character at a time
        if (bytes_received == -1) {
            // If recv() failed, throw an exception with details from errno
            throw SocketException("Receiving message failed: " + std::string(strerror(errno)));
        } else if (bytes_received == 0) {
            // If recv() returns 0, the connection has been gracefully closed
            throw SocketException("Peer has closed the connection");
        } else {
            // Check for the delimiter
            if (ch == '\n') {
                break; // Delimiter found, exit the loop
            }
            message += ch; // Append the character to the message
        }
    }
    return message; // Return the received message without the delimiter
}

void MessageSocket::close() {
    if (is_open_) {
        ::close(file_descriptor_);
        is_open_ = false;
    }
    // Perform any additional cleanup if required.
}
