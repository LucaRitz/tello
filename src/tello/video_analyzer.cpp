#include "tello/video_analyzer.hpp"
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

#include <iostream>

bool tello::VideoAnalyzer::append(const string& framePart) {
    _frame += framePart;
    bool isFinish = _frame.size() % VIDEO_PACKET_LENGTH != 0;
    if (isFinish) {
        std::cout << string_to_hex(_frame) << std::endl;
    }
    return isFinish;
}

string tello::VideoAnalyzer::frame() const {
    return _frame;
}

void tello::VideoAnalyzer::clean() {
    _frame.clear();
}

std::string tello::VideoAnalyzer::string_to_hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}