#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <future>

using std::unordered_map;
using std::string;
using std::shared_ptr;
using std::mutex;
using std::function;
using std::promise;

namespace tello {
    class Response;
}

using t_subscriber =  std::function<void (const tello::Response&)>;

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

        [[nodiscard]] Status status() const;

        static shared_ptr<Response> error();
        static shared_ptr<Response> empty();

        virtual void update(const string& value);
        void update(const Status& status);
        promise<const Response&>& subscribe();

    protected:
        void callSubscriber();
        Status _status;

    private:
        promise<const Response&> _promise;
        mutex _mutex;
    };
}