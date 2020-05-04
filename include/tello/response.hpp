#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>

using std::unordered_map;
using std::string;
using std::shared_ptr;
using std::mutex;

namespace tello {
    class Response;
}

namespace tello {

    enum class Status {
        OK,
        FAIL,
        TIMEOUT,
        UNKNOWN
    };

    class Response {
    public:
        Response() : _status(Status::UNKNOWN){}
        explicit Response(const string& value);
        explicit Response(const Status& status);

        [[nodiscard]] Status status() const;

    protected:
        Status _status;
    };
}