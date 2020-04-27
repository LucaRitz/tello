#pragma once

#include <unordered_map>
#include <thread>
#include <future>
#include <string>
#include "../logger/logger.hpp"
#include "connection_data.hpp"
#include <mutex>
#include <shared_mutex>

#define UDP_BUFFER_LENGTH 4096

using ip_address = unsigned long;
using std::unordered_map;
using std::thread;
using std::promise;
using std::future;
using tello::Logger;
using tello::LoggerType;

namespace tello {

    class Tello;

    template<typename Response, Response (* factory)(const char* const), void (* invoke)(const Response&, const Tello& tello)>
    class UdpListener {
    public:
        UdpListener(const ConnectionData& connectionData,
                    unordered_map<ip_address, const Tello*>& telloMapping, std::shared_mutex& telloMappingMutex,
                    std::shared_mutex& connectionMutex, LoggerType loggerType)
                : _exitSignal(),
                  _worker(thread(&UdpListener::listen, std::ref(connectionData), std::ref(telloMapping),
                                 std::ref(telloMappingMutex), std::ref(connectionMutex),
                                 _exitSignal.get_future(), loggerType)) {
        }

        void stop() {
            _exitSignal.set_value();
        }

    private:
        promise<void> _exitSignal;
        const thread _worker;

        static void listen(const tello::ConnectionData& connectionData,
                           unordered_map<ip_address, const Tello*>& telloMapping, std::shared_mutex& telloMappingMutex,
                           std::shared_mutex& connectionMutex, future<void> exitListener,
                           LoggerType loggerType) {
            char buffer[UDP_BUFFER_LENGTH];
            int n;
            sockaddr_in sender{};
            int senderAddrSize = sizeof(sender);
            memset(&sender, 0, sizeof(sender));
            bool isFirstAccessToFileDescriptor = true;

            while (exitListener.wait_for(std::chrono::milliseconds(30)) == std::future_status::timeout) {
                connectionMutex.lock_shared();
                if (connectionData._fileDescriptor == -1) {
                    connectionMutex.unlock_shared();
                    continue;
                } else if (isFirstAccessToFileDescriptor) {
                    Logger::get(loggerType)->info(string("Start listen to port {0:d}"),
                                                  ntohs(connectionData._servaddr.sin_port));
                    isFirstAccessToFileDescriptor = false;
                }

                int port = ntohs(connectionData._servaddr.sin_port);
                n = recvfrom(connectionData._fileDescriptor, (char*) buffer, UDP_BUFFER_LENGTH, 0,
                             (struct sockaddr*) &sender, &senderAddrSize);
                connectionMutex.unlock_shared();

                n = n >= 0 ? n : 0;
                buffer[n] = '\0';

                telloMappingMutex.lock_shared();

                auto telloIt = telloMapping.find(sender.sin_addr.s_addr);
                if (telloIt != telloMapping.end()) {
                    Response res = factory(buffer);
                    invoke(res, *(telloIt->second));
                } else {
                    Logger::get(loggerType)->warn(string("Received data {0:d} from unknown Tello {0:x}"), n,
                                                  sender.sin_addr.s_addr);
                }
                telloMappingMutex.unlock_shared();
            }

            Logger::get(loggerType)->info(string("Stop listen to port {0:d}"),
                                          ntohs(connectionData._servaddr.sin_port));
        }
    };
}