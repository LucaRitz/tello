#include "network_interface_factory.hpp"
#include <tello/native/network_interface.hpp>

using tello::NetworkInterface;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    #include "windows/network_impl.hpp"

    using tello::windows::NetworkImpl;

    unique_ptr<NetworkInterface> tello::NetworkInterfaceFactory::build() {
        return std::make_unique<NetworkImpl>();
    }
#endif