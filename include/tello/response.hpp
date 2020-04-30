#pragma once

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

namespace tello {

    enum class Status {
        OK,
        FAIL,
        TIMEOUT,
        UNKNOWN
    };

    class Response {
    public:
        explicit Response(const Status& status);
        explicit Response(const string& response);

        Status status();
        [[nodiscard]]
        string param(const string& key) const;

    protected:
        void append(const string& key, const string& value);

    private:
        Status _status;
        unordered_map<string, string> _values;
    };
}