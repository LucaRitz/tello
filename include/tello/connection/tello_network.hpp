#pragma once

#include "../macro_definition.hpp"

namespace tello {
    class EXPORT TelloNetwork {
    public:
        TelloNetwork() = delete;
        TelloNetwork(const TelloNetwork&) = delete;
        TelloNetwork(TelloNetwork&&) = delete;

        static bool connect();
        static void disconnect();
    };
}
