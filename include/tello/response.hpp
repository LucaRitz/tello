#pragma once

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

namespace tello {

    enum class Status {
        OK,
        FAIL
    };

    class Response {
    public:
        explicit Response(Status status);

        Status status();
        string param(string key);

    protected:
        void append(string key, string value);

    private:
        const Status _status;
        unordered_map<string, string> _values;
    };
}