#pragma once

#include <unordered_map>
#include <thread>
#include <future>
#include <string>
#include "../logger/logger.hpp"
#include "connection_data.hpp"
#include "../response/status_response.hpp"
#include <mutex>
#include <shared_mutex>

#include <WinSock2.h>

#define BUFFER_LENGTH 1024

using ip_address = unsigned long;
using std::unordered_map;
using std::thread;
using std::promise;
using std::future;
using tello::Logger;
using tello::LoggerType;
using tello::Response;

namespace tello {

    class Tello;

    template<typename Response, Response (* factory)(const char* const), void (* invoke)(const Response&, const Tello& tello)>
    class UdpListener {
    public:
        UdpListener(const ConnectionData& connectionData,
                    unordered_map<ip_address, const Tello*>& telloMapping, std::shared_mutex& telloMappingMutex,
                    std::shared_mutex& connectionMutex)
                : _exitSignal(),
                  _worker(thread(&UdpListener::listen, std::ref(connectionData), std::ref(telloMapping),
                                 std::ref(telloMappingMutex), std::ref(connectionMutex), _exitSignal.get_future())) {
        }

        void stop() {
            _exitSignal.set_value();
        }

    private:
        promise<void> _exitSignal;
        const thread _worker;

        static void listen(const tello::ConnectionData& connectionData,
                           unordered_map<ip_address, const Tello*>& telloMapping, std::shared_mutex& telloMappingMutex,
                           std::shared_mutex& connectionMutex, future<void> exitListener) {
            Logger::get(LoggerType::STATUS)->info(
                    string("Start listen to port {0:d}"), ntohs(connectionData._servaddr.sin_port));

            char buffer[BUFFER_LENGTH];
            int n;
            int len = 0;
            sockaddr_in sender{};
            memset(&sender, 0, sizeof(sender));

            while (exitListener.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {

                connectionMutex.lock_shared();
                if (connectionData._fileDescriptor == -1) {
                    connectionMutex.unlock_shared();
                    continue;
                }

                n = recvfrom(connectionData._fileDescriptor, (char*) buffer, BUFFER_LENGTH, MSG_WAITALL,
                             (struct sockaddr*) &sender, &len);
                connectionMutex.unlock_shared();

                buffer[n] = '\0';

                telloMappingMutex.lock_shared();

                auto telloIt = telloMapping.find(sender.sin_addr.s_addr);
                if (telloIt == telloMapping.end()) {
                    Logger::get(LoggerType::STATUS)->warn(string("Received data from unknown Tello {0:x}"),
                                                           sender.sin_addr.s_addr);
                    continue;
                }
                Response res = factory(buffer);
                invoke(res, *(telloIt->second));

                telloMappingMutex.unlock_shared();
            }


            connectionMutex.lock_shared();
            Logger::get(LoggerType::STATUS)->info(
                    string("Stop listen to port {0:d}"), ntohs(connectionData._servaddr.sin_port));

            if (connectionData._fileDescriptor != -1) {
                // TODO: Close connection
            }
            connectionMutex.unlock_shared();
        }
    };
}