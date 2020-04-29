#include "response_factory.hpp"
#include <tello/response.hpp>
#include <tello/logger/logger.hpp>
#include <tello/command_type.hpp>

using tello::Response;
using tello::ResponseBuildType;
using tello::CommandType;
using tello::EnumClassHash;
using tello::Status;
using std::unique_ptr;
using tello::Logger;

const unordered_map<const CommandType, ResponseBuildType, EnumClassHash> tello::ResponseFactory::MAPPING = ResponseFactory::createMap();

unordered_map<const CommandType, ResponseBuildType, EnumClassHash> tello::ResponseFactory::createMap() {
    unordered_map<const CommandType, ResponseBuildType, EnumClassHash> mapping;
    mapping[CommandType::COMMAND] = ResponseFactory::simpleResponse;
    mapping[CommandType::TAKE_OFF] = ResponseFactory::simpleResponse;
    mapping[CommandType::LAND] = ResponseFactory::simpleResponse;
    mapping[CommandType::UP] = ResponseFactory::simpleResponse;
    // TODO: Add Command-Mappings
    return mapping;
}

unique_ptr<Response> tello::ResponseFactory::build(const CommandType& commandType, string &response) {
    auto value = tello::ResponseFactory::MAPPING.find(commandType);
    if (value != tello::ResponseFactory::MAPPING.end()) {
        return value->second(commandType, response);
    }

    Logger::get(LoggerType::COMMAND)->critical(
            std::string("Response for type [{}] not found!"), NAMES.find(commandType)->second);
    return std::make_unique<Response>(Status::FAIL);
}

unique_ptr<Response> tello::ResponseFactory::timeout() {
    return std::make_unique<Response>(Status::TIMEOUT);
}

unique_ptr<Response> tello::ResponseFactory::error() {
    return std::make_unique<Response>(Status::FAIL);
}

unique_ptr<Response> tello::ResponseFactory::simpleResponse(const CommandType& commandType, string& response) {
    return std::make_unique<Response>(response);
}