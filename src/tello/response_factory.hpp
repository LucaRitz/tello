#pragma once

#include <string>
#include <tello/command_type.hpp>
#include <memory>
#include <unordered_map>

using std::string;
using std::unique_ptr;
using tello::EnumClassHash;
using tello::CommandType;
using std::unordered_map;

namespace tello {

    class Response;

    using ResponseBuildType = unique_ptr<Response> (*)(const CommandType&, string&);

    class ResponseFactory {
    public:
        ResponseFactory() = delete;
        ResponseFactory(const ResponseFactory&) = delete;
        ResponseFactory(const ResponseFactory&&) = delete;

        static unique_ptr<Response> build(const CommandType& commandType, string& response);
        static unique_ptr<Response> timeout();
        static unique_ptr<Response> error();

    private:
        static const unordered_map<const CommandType, ResponseBuildType, EnumClassHash> MAPPING;
        static unordered_map<const CommandType, ResponseBuildType, EnumClassHash> createMap();

        static unique_ptr<Response> simpleResponse(const CommandType& commandType, string& response);
    };
}