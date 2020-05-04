#pragma once

#include <unordered_map>
#include <thread>
#include <future>
#include <string>
#include "../logger/logger.hpp"
#include "../native/network_interface.hpp"
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <vector>
#include <chrono>

using ip_address = unsigned long;
using std::unordered_map;
using std::thread;
using std::promise;
using std::future;
using tello::Logger;
using tello::LoggerType;
using std::shared_ptr;
using std::vector;

namespace tello {

    class Response;

    struct ResponseMapping {
        ResponseMapping(shared_ptr<Response> response, time_t insertDate) :
                _response(std::move(response)), _insertDate(insertDate) {}

        shared_ptr<Response> _response;
        time_t _insertDate;
    };

    class UdpCommandListener {
    public:
        UdpCommandListener(const ConnectionData& connectionData, shared_ptr<NetworkInterface> networkInterface,
                           std::shared_mutex& connectionMutex);

        void stop();

        template <typename Response>
        void append(const unordered_map<ip_address, shared_ptr<Response>>& responses) {
            _responseMutex.lock();
            auto now = UdpCommandListener::currentTime();
            for(auto& entry : responses) {
                auto responseEntry = _mapping.find(entry.first);
                if (responseEntry != _mapping.end()) {
                    responseEntry->second.emplace_back(entry.second, now);
                } else {
                    vector<ResponseMapping> responseMapping {ResponseMapping{entry.second, now}};
                    _mapping[entry.first] = responseMapping;
                }
            }
            _responseMutex.unlock();
        }

        static vector<ResponseMapping> cleanMapping(unordered_map<ip_address, vector<ResponseMapping>>& mapping);

    private:
        promise<void> _exitSignal;
        unordered_map<ip_address, vector<ResponseMapping>> _mapping;
        std::mutex _responseMutex;
        thread _worker;

        static void listen(const ConnectionData& connectionData, shared_ptr<NetworkInterface> networkInterface,
                           std::shared_mutex& connectionMutex, future<void> exitListener,
                           unordered_map<ip_address, vector<ResponseMapping>>& mapping, std::mutex& responseMutex);
        static time_t currentTime();
    };
}