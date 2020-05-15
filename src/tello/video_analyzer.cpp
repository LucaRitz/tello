#include "tello/video_analyzer.hpp"
#include "tello/logger/logger.hpp"
#include <string>

#define VIDEO_PACKET_LENGTH 1460

using std::string;

#include <iostream>

tello::VideoAnalyzer::VideoAnalyzer() : _frame(nullptr), _currentSize(0) {

}

tello::VideoAnalyzer::~VideoAnalyzer() {
    clean();
}

bool tello::VideoAnalyzer::append(const unsigned char* const& framePart, int length) {
    auto* temp = new unsigned char[_currentSize + length];
    if (_frame != nullptr) {
        memcpy(temp, _frame, _currentSize);
        delete[] _frame;
    }
    memcpy(temp + _currentSize, framePart, length);
    _frame = temp;
    _currentSize += length;

    bool isFinish = _currentSize % VIDEO_PACKET_LENGTH != 0;
    if (isFinish) {
        std::cout << "\n\n" << string_to_hex(_frame, _currentSize) << std::endl;
    }
    return isFinish;
}

unsigned char* tello::VideoAnalyzer::frame() const {
    return _frame;
}

unsigned int tello::VideoAnalyzer::length() const {
    return _currentSize;
}

void tello::VideoAnalyzer::clean() {
    delete[] _frame;
    _frame = nullptr;
    _currentSize = 0;
}

std::string tello::VideoAnalyzer::string_to_hex(unsigned char*& input, unsigned int size)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(size * 2);
    for (int i = 0; i < size; i++)
    {
        output.push_back(hex_digits[input[i] >> 4]);
        output.push_back(hex_digits[input[i] & 15]);
    }
    return output;
}