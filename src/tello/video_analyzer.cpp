#include "tello/video_analyzer.hpp"
#include "tello/logger/logger.hpp"
#include <tello/native/network_interface.hpp>
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

tello::VideoAnalyzer::VideoAnalyzer() : _frames() {

}

tello::VideoAnalyzer::~VideoAnalyzer() {
    _frames.clear();
}

bool tello::VideoAnalyzer::isStart(const char* const& framePart, int length) {
    return length >= 4 && framePart[0] == '\0' && framePart[1] == '\0' && framePart[2] == '\0' && framePart[3] == '\1';
}

bool tello::VideoAnalyzer::append(NetworkResponse& response) {
    if(_frames.find(response._sender._ip) == _frames.end()) {
        _frames[response._sender._ip] = std::make_shared<vector<NetworkResponse>>();
    }

    auto frames = _frames.find(response._sender._ip);

    if(frames->second->empty() && !isStart(response._response, response._length)) {
        return false;
    }

    int length = response._length;
    frames->second->push_back(std::move(response));

    return length < VIDEO_PACKET_LENGTH;
}

unsigned char* tello::VideoAnalyzer::frame(ip_address address) const {
    unsigned int leng = length(address);
    auto* frame = new unsigned char[length(address)];
    int pos = 0;

    auto frames = _frames.find(address);
    for(const auto& response : *frames->second) {
        memcpy(frame + pos, response._response, response._length);
        pos += response._length;
    }
    return frame;
}

unsigned int tello::VideoAnalyzer::length(ip_address address) const {
    int size = 0;

    auto frames = _frames.find(address);
    for(const auto& frame : *frames->second) {
        size += frame._length;
    }

    return size;
}

void tello::VideoAnalyzer::clean(ip_address address) {
    _frames.erase(address);
}