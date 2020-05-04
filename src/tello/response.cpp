#include <tello/response.hpp>

using tello::Status;

tello::Response::Response(const Status& status) : _status(status), _promise(), _mutex() {
    if (Status::UNKNOWN != _status) {
        callSubscriber();
    }
}

Status tello::Response::status() const {
    return _status;
}

shared_ptr<tello::Response> tello::Response::error() {
    return std::make_shared<Response>(Status::FAIL);
}

shared_ptr<tello::Response> tello::Response::empty() {
    return std::make_shared<Response>(Status::UNKNOWN);
}

void tello::Response::update(const string& value) {
    _status = value.find(string("ok")) != std::string::npos ? Status::OK : (value.find(string("error")) !=
                                                                     std::string::npos ? Status::FAIL
                                                                                       : Status::UNKNOWN);
    callSubscriber();
}

void tello::Response::update(const Status &status) {
    _mutex.lock();
    _status = status;
    _mutex.unlock();
    callSubscriber();
}

promise<const tello::Response&>& tello::Response::subscribe() {
    return _promise;
}

void tello::Response::callSubscriber() {
    _promise.set_value(*this);
}