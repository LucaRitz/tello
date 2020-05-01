#include "tello/video_analyzer.hpp"
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

bool tello::VideoAnalyzer::append(const string& framePart) {
    _frame += framePart;
    return _frame.size() % VIDEO_PACKET_LENGTH != 0;
}

string tello::VideoAnalyzer::frame() const {
    return _frame;
}

void tello::VideoAnalyzer::clean() {
    _frame.clear();
}