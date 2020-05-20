#include "tello/native/network_interface.hpp"

tello::NetworkData::NetworkData(const tello::SIN_FAM sinFam, const unsigned short port, const ip_address ip) :
        _sinFam(sinFam),
        _port(port),
        _ip(ip) {}

tello::ConnectionData::ConnectionData(int fileDescriptor, NetworkData networkData) :
        _fileDescriptor(fileDescriptor),
        _networkData(networkData) {}

tello::NetworkResponse::NetworkResponse(const tello::NetworkData& sender, char* response, int size) :
        _sender(sender),
        _response(response),
        _length(size){}

tello::NetworkResponse::NetworkResponse(const NetworkResponse& other) :
        _sender(other._sender),
        _length(other._length) {
    _response = reinterpret_cast<char*>(std::memcpy(new char[other._length], other._response,
                                                           other._length));
}

tello::NetworkResponse& tello::NetworkResponse::operator=(const NetworkResponse& other) {
    if (this == &other) {
        return *this;
    }

    this->_sender = other._sender;
    delete[] this->_response;
    this->_response = reinterpret_cast<char*>(std::memcpy(new char[other._length], other._response,
                                                                   other._length));
    this->_length = other._length;

    return *this;
}

tello::NetworkResponse::NetworkResponse(tello::NetworkResponse&& other) noexcept :
        _sender(other._sender),
        _response(other._response),
        _length(other._length) {
    other._sender = NetworkData();
    other._length = 0;
    other._response = nullptr;
}

tello::NetworkResponse& tello::NetworkResponse::operator=(NetworkResponse&& other) noexcept {
    this->_sender = other._sender;
    this->_response = other._response;
    this->_length = other._length;

    other._sender = NetworkData();
    other._response = nullptr;
    other._length = 0;

    return *this;
}

tello::NetworkResponse::~NetworkResponse() {
    delete[] _response;
    _response = nullptr;
}

string tello::NetworkResponse::response() const {
    if (_response != nullptr) {
        return string(_response);
    }
    return "";
}