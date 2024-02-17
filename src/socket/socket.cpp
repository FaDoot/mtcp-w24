#include "socket/socket.hpp"


Socket::Socket() : file_descriptor_(-1) {
}

Socket::Socket(int file_descriptor) : file_descriptor_(file_descriptor) {
}

Socket::~Socket() = default;