#pragma once

#include <unordered_map>
#include <thread>
#include <future>
#include <string>
#include "tello/logger/logger_interface.hpp"
#include "tello/native/network_interface.hpp"
#include <mutex>
#include <shared_mutex>
#include <memory>

using ip_address = unsigned long;
using std::unordered_map;
using std::thread;
using std::promise;
using std::future;
using tello::LoggerInterface;
using tello::LoggerType;
using std::shared_ptr;

namespace tello {

    class Tello;

    template<void (* invoke)(NetworkResponse&, const Tello* tello)>
    class UdpListener {
    public:
        UdpListener(const ConnectionData& connectionData, shared_ptr<NetworkInterface> networkInterface,
                    unordered_map<ip_address, const Tello*>& telloMapping, std::shared_mutex& telloMappingMutex,
                    std::shared_mutex& connectionMutex, LoggerType loggerType)
                : _exitSignal(),
                  _worker(thread(&UdpListener::listen, std::ref(connectionData), networkInterface, std::ref(telloMapping),
                                 std::ref(telloMappingMutex), std::ref(connectionMutex),
                                 _exitSignal.get_future(), loggerType)) {
        }

        void stop() {
            _exitSignal.set_value();
            _worker.join();
        }

    private:
        promise<void> _exitSignal;
        thread _worker;

        static void listen(const tello::ConnectionData& connectionData, shared_ptr<NetworkInterface> networkInterface,
                           unordered_map<ip_address, const Tello*>& telloMapping, std::shared_mutex& telloMappingMutex,
                           std::shared_mutex& connectionMutex, future<void> exitListener,
                           LoggerType loggerType) {
            bool isFirstAccessToFileDescriptor = true;

            while (exitListener.wait_for(std::chrono::nanoseconds(100)) == std::future_status::timeout) {
                connectionMutex.lock_shared();
                if (connectionData._fileDescriptor == -1) {
                    connectionMutex.unlock_shared();
                    continue;
                } else if (isFirstAccessToFileDescriptor) {
                    LoggerInterface::info(loggerType, string("Start listen to port {}"),
                                                           std::to_string(connectionData._networkData._port));
                    isFirstAccessToFileDescriptor = false;
                }

                NetworkResponse networkResponse = networkInterface->read(connectionData._fileDescriptor);
                connectionMutex.unlock_shared();

                telloMappingMutex.lock_shared();

                auto telloIt = telloMapping.find(networkResponse._sender._ip);
                if (telloIt != telloMapping.end()) {
                    invoke(networkResponse, telloIt->second);
                } else if (networkResponse._length > 0) {
                    LoggerInterface::warn(loggerType, string("Received data {0} from unknown Tello {1}"),
                                                           networkResponse.response(), std::to_string(networkResponse._sender._ip));
                }
                telloMappingMutex.unlock_shared();
            }

            LoggerInterface::info(loggerType, string("Stop listen to port {0}"),
                                                   std::to_string(connectionData._networkData._port));
        }
    };
}