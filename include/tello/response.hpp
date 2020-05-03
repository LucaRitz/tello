#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <functional>

using std::unordered_map;
using std::string;
using std::shared_ptr;
using std::mutex;
using std::function;

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
        static shared_ptr<Response> timeout();
        static shared_ptr<Response> empty();


        virtual void update(const string& value);
        void update(const Status& status);
        void subscribe(const t_subscriber& subscriber);

    protected:
        void callSubscriber();
        Status _status;

    private:
        t_subscriber _subscriber;
        mutex _mutex;
    };
}