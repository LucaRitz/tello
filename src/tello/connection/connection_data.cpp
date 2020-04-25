#include "tello/connection/connection_data.hpp"

tello::ConnectionData::ConnectionData(int fileDescriptor, struct sockaddr_in servaddr) : _fileDescriptor(
        fileDescriptor), _servaddr(servaddr) {};