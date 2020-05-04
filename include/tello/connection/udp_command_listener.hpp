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
#include "../response.hpp"
#include "../response/query_response.hpp"

using ip_address = unsigned long;
using std::unordered_map;
using std::thread;
using std::promise;
using std::future;
using tello::Logger;
using tello::LoggerType;
using std::shared_ptr;
using std::vector;
using tello::Response;
using tello::QueryResponse;

namespace tello {

    enum class ResponseMappingType {
        RESPONSE,
        QUERY_RESPONSE
    };

    struct ResponseMapping {

        ResponseMapping(Response response, promise<Response>& prom, time_t insertDate);
        ResponseMapping(QueryResponse response, promise<QueryResponse>& prom, time_t insertDate);

        void set_value(const string& value);
        void set_value(const Status& status);
        void complete();

        time_t _insertDate;

    private:
        promise<Response> _prom;
        promise<QueryResponse> _queryProm;
        Response _response;
        QueryResponse _queryResponse;
        ResponseMappingType _responseMappingType;
    };

    class UdpCommandListener {
    public:
        UdpCommandListener(const ConnectionData& connectionData, shared_ptr<NetworkInterface> networkInterface,
                           std::shared_mutex& connectionMutex);

        void stop();

        template <typename Response>
        unordered_map<ip_address, future<Response>> append(const unordered_map<ip_address, Response>& responses) {
            _responseMutex.lock();
            unordered_map<ip_address, future<Response>> futures{};
            auto now = UdpCommandListener::currentTime();
            for(auto& entry : responses) {
                auto responseEntry = _mapping.find(entry.first);
                promise<Response> prom{};
                futures[entry.first] = std::move(prom.get_future());
                if (responseEntry != _mapping.end()) {
                    responseEntry->second.push_back(std::make_shared<ResponseMapping>(entry.second, prom, now));
                } else {
                    vector<shared_ptr<ResponseMapping>> responseMapping{
                            std::make_shared<ResponseMapping>(entry.second, prom, now)};
                    _mapping[entry.first] = std::move(responseMapping);
                }
            }
            _responseMutex.unlock();
            return futures;
        }

    private:
        promise<void> _exitSignal;
        unordered_map<ip_address, vector<shared_ptr<ResponseMapping>>> _mapping;
        std::mutex _responseMutex;
        thread _worker;

        static vector<shared_ptr<ResponseMapping>> cleanMapping(unordered_map<ip_address, vector<shared_ptr<ResponseMapping>>>& mapping);
        static void listen(const ConnectionData& connectionData, shared_ptr<NetworkInterface> networkInterface,
                           std::shared_mutex& connectionMutex, future<void> exitListener,
                           unordered_map<ip_address, vector<shared_ptr<ResponseMapping>>>& mapping, std::mutex& responseMutex);
        static time_t currentTime();
    };
}