#pragma once

#include <memory>

using std::unique_ptr;

namespace tello {

    class NetworkInterface;

    class NetworkInterfaceFactory {
    public:
        static unique_ptr<NetworkInterface> build();

    private:
        NetworkInterfaceFactory() = default;
    };
}