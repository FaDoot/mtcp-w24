#include "socket/socket.hpp"


// Protected constructor, not to be used directly
Socket::Socket() : file_descriptor_(-1) {
}

// Constructor taking a file descriptor for connection sockets
Socket::Socket(int file_descriptor) : file_descriptor_(file_descriptor) {
}

// Virtual destructor – must be implemented even if it's pure virtual
Socket::~Socket() = default;