#include <tello/response.hpp>

using tello::Status;

tello::Response::Response(Status status) : _status(status) {}

Status tello::Response::status() {
    return _status;
}

string tello::Response::param(string key) {
    auto it = _values.find(key);
    if (it != _values.end()) {
        return _values.find(key)->second;
    }

    return string("");
}

void tello::Response::append(string key, string value) {
    _values[key] = value;
}