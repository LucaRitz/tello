#include <tello/response.hpp>

using tello::Status;

tello::Response::Response(const Status& status) : _status(status) {}

tello::Response::Response(const string& value) : Response(
        value.find(string("ok")) != std::string::npos ? Status::OK : (value.find(string("error")) !=
                                                                      std::string::npos ? Status::FAIL
                                                                                        : Status::UNKNOWN)) {}

Status tello::Response::status() const {
    return _status;
}