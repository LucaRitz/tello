#include "tello/video_analyzer.hpp"
#include <cstring>
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

void tello::VideoAnalyzer::append(const string& framePart) {
    _frame += framePart;
}

bool tello::VideoAnalyzer::finishedFrame() const {
    return _frame.size() % VIDEO_PACKET_LENGTH != 0;
}

string tello::VideoAnalyzer::frame() const {
    return _frame;
}

void tello::VideoAnalyzer::clean() {
    _frame.clear();
}