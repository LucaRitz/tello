#include "tello/video_analyzer.hpp"
#include "tello/logger/logger.hpp"
#include <tello/native/network_interface.hpp>
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

tello::VideoAnalyzer::VideoAnalyzer() : _frames(), _frameSizes() {

}

tello::VideoAnalyzer::~VideoAnalyzer() {
    clean();
}

bool tello::VideoAnalyzer::isStart(const char* const& framePart, int length) {
    return length >= 4 && framePart[0] == '\0' && framePart[1] == '\0' && framePart[2] == '\0' && framePart[3] == '\1';
}

bool tello::VideoAnalyzer::append(NetworkResponse& response) {
    if(_frames.empty() && !isStart(response._response, response._length)) {
        return false;
    }

    int length = response._length;
    _frames.push_back(std::move(response));

    return length % VIDEO_PACKET_LENGTH != 0;
}

unsigned char* tello::VideoAnalyzer::frame() const {
    auto* frame = new unsigned char[length()];
    int pos = 0;

    for(int i = 0; i < _frames.size(); i++) {
        const NetworkResponse& response = _frames.at(i);
        memcpy(frame + pos, response._response, response._length);
        pos += response._length;
    }
    return frame;
}

unsigned int tello::VideoAnalyzer::length() const {
    int size = 0;
    for(auto& frame : _frames) {
        size += frame._length;
    }

    return size;
}

void tello::VideoAnalyzer::clean() {
    _frames.clear();
}