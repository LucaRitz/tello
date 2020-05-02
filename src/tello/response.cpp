#include <tello/response.hpp>

using tello::Status;

tello::Response::Response(const Status& status) : _status(status) {}

tello::Response::Response(const string& response) : Response(
        response.find(string("ok")) != std::string::npos ? Status::OK : (response.find(string("error")) !=
                                                                         std::string::npos ? Status::FAIL
                                                                                           : Status::UNKNOWN)
) {}

Status tello::Response::status() const {
    return _status;
}

unique_ptr<tello::Response> tello::Response::error() {
    return std::make_unique<Response>(Status::FAIL);
}

unique_ptr<tello::Response> tello::Response::timeout() {
    return std::make_unique<Response>(Status::TIMEOUT);
}

unique_ptr<tello::Response> tello::Response::of(const string& arg) {
    return std::make_unique<Response>(arg);
}