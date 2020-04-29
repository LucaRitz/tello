#include <tello/response.hpp>

using tello::Status;

tello::Response::Response(const Status& status) : _status(status) {}

tello::Response::Response(const string& response) : Response(
        response.find(string("ok")) != std::string::npos ? Status::OK : (response.find(string("error")) !=
                                                                         std::string::npos ? Status::FAIL
                                                                                           : Status::UNKNOWN)
) {}

Status tello::Response::status() {
    return _status;
}

string tello::Response::param(const string& key) const {
    auto it = _values.find(key);
    if (it != _values.end()) {
        return _values.find(key)->second;
    }

    return string("");
}

void tello::Response::append(const string& key, const string& value) {
    _values[key] = value;
}