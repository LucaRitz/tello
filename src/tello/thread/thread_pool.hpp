#pragma once

#include <functional>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

using ip_address = unsigned long;

namespace tello {
    class NetworkResponse;
    class Tello;
    enum class LoggerType;
}

namespace tello::threading {

    class ThreadPoolImpl;

    class Threadpool {
    public:
        Threadpool();
        ~Threadpool();

        void push(std::function<void(int)>&& function);
        void stop();

    private:
        std::unique_ptr<ThreadPoolImpl> _impl;
    };
}