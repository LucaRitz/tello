#include "tello/video_analyzer.hpp"
#include "tello/logger/logger.hpp"
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

#include <iostream>

tello::VideoAnalyzer::VideoAnalyzer() : _frames(), _frameSizes() {

}

tello::VideoAnalyzer::~VideoAnalyzer() {
    clean();
}

bool tello::VideoAnalyzer::isStart(const char* const& framePart, int length) {
    return length >= 4 && framePart[0] == '\0' && framePart[1] == '\0' && framePart[2] == '\0' && framePart[3] == '\1';
}

bool tello::VideoAnalyzer::append(const char* const& framePart, int length) {
    if(_frames.empty() && !isStart(framePart, length)) {
        return false;
    }

    auto* frame = new unsigned char[length];
    memcpy(frame, framePart, length);
    _frames.push_back(frame);
    _frameSizes.push_back(length);

    return length % VIDEO_PACKET_LENGTH != 0;
}

unsigned char* tello::VideoAnalyzer::frame() const {
    auto* frame = new unsigned char[length()];
    int pos = 0;
    for(int i = 0; i < _frames.size(); i++) {
        memcpy(frame + pos, _frames.at(i), _frameSizes.at(i));
        pos += _frameSizes.at(i);
    }
    return frame;
}

unsigned int tello::VideoAnalyzer::length() const {
    int size = 0;
    for(int frameSize : _frameSizes) {
        size += frameSize;
    }

    return size;
}

void tello::VideoAnalyzer::clean() {
    for(auto* frame : _frames) {
        delete[] frame;
    }
    _frames.clear();
    _frameSizes.clear();
}