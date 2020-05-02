#pragma once

#include <unordered_map>
#include <string>
#include <memory>

using std::unordered_map;
using std::string;
using std::unique_ptr;

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

        [[nodiscard]] Status status() const;

        static unique_ptr<Response> error();
        static unique_ptr<Response> timeout();
        static unique_ptr<Response> of(const string& arg);

    protected:
        Status _status;
    };
}