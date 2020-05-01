#include "tello/video_analyzer.hpp"
#include <cstring>
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

void tello::VideoAnalyzer::append(const string& framePart) {
    _currentSize += framePart.size();
    _frames.push_back(framePart);
}

bool tello::VideoAnalyzer::finishedFrame() const {
    return _currentSize % VIDEO_PACKET_LENGTH != 0;
}

string tello::VideoAnalyzer::frame() const {
    std::string frame;
    for (const string& framePart : _frames) {
        frame += framePart;
    }

    return frame;
}

void tello::VideoAnalyzer::clean() {
    _frames.clear();
    _currentSize = 0;
}