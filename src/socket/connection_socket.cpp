#include "socket/connection_socket.hpp"
//#include "socket.hpp"
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

ConnectionSocket::ConnectionSocket(int file_descriptor) : Socket(file_descriptor) {
}


ConnectionSocket::~ConnectionSocket() {

     
    close(file_descriptor_);
    //std::cout << "is it happening at close" <<std::endl;
}