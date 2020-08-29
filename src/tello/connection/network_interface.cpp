#include <tello/connection/tello_network.hpp>
#include "network.hpp"

bool tello::TelloNetwork::connect() {
    return Network::connect();
}

void tello::TelloNetwork::disconnect() {
    Network::disconnect();
}