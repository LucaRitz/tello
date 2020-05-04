#include <tello/response.hpp>

using tello::Status;

tello::Response::Response(const Status& status) : _status(status) {}

Status tello::Response::status() const {
    return _status;
}

tello::Response tello::Response::error() {
    return Response{Status::FAIL};
}

tello::Response tello::Response::empty() {
    return Response{Status::UNKNOWN};
}

void tello::Response::update(const string& value) {
    _status = value.find(string("ok")) != std::string::npos ? Status::OK : (value.find(string("error")) !=
                                                                     std::string::npos ? Status::FAIL
                                                                                       : Status::UNKNOWN);
}

void tello::Response::update(const Status &status) {
    _status = status;
}