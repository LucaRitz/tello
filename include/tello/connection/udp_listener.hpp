#pragma once

#include <unordered_map>
#include <thread>
#include <future>
#include <string>
#include <tello/logger/logger.hpp>
#include <tello/connection/connection_data.hpp>
#include <tello/response.hpp>

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
                    unordered_map<ip_address, const Tello*>& telloMapping)
                : _exitSignal(),
                  _worker(thread(&UdpListener::listen, connectionData, telloMapping, _exitSignal.get_future())) {
        }

        void stop() {
            _exitSignal.set_value();
        }

    private:
        promise<void> _exitSignal;
        const thread _worker;

        static void listen(const tello::ConnectionData& connectionData,
                    unordered_map<ip_address, const Tello*>& telloMapping,
                    future<void> exitListener) {
            Logger::get(LoggerType::STATUS)->info(
                    string("Start listen to port {0:d}"), ntohs(connectionData._servaddr.sin_port));
            while (exitListener.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {

                if (connectionData._fileDescriptor == -1) {
                    continue;
                }

                char buffer[BUFFER_LENGTH];
                int n;
                int len = 0;
                sockaddr_in sender{};
                memset(&sender, 0, sizeof(sender));

                n = recvfrom(connectionData._fileDescriptor, (char*) buffer, BUFFER_LENGTH, MSG_WAITALL,
                             (struct sockaddr*) &sender, &len);
                buffer[n] = '\0';

                auto telloIt = telloMapping.find(sender.sin_addr.s_addr);
                if (telloIt == telloMapping.end()) {
                    Logger::get(LoggerType::STATUS)->warn(string("Received data from unknown Tello {0:x}"),
                                                           sender.sin_addr.s_addr);
                    continue;
                }

                Response res = factory(buffer);
                invoke(res, *(telloIt->second));
            }


            Logger::get(LoggerType::STATUS)->info(
                    string("Stop listen to port {0:d}"), ntohs(connectionData._servaddr.sin_port));

            // TODO: Close connection
        }
    };
}