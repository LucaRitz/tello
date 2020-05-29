#pragma once

#include <ctpl_stl.h>

#define THREADS 1

namespace tello::threading {

    class ThreadPoolImpl {
    public:
        ThreadPoolImpl() : _pool(THREADS) {}
        ThreadPoolImpl(const ThreadPoolImpl&) = delete;
        ThreadPoolImpl& operator=(const ThreadPoolImpl&) = delete;
        ThreadPoolImpl(ThreadPoolImpl&&) = delete;
        ThreadPoolImpl& operator=(ThreadPoolImpl&&) = delete;

        template<typename F, typename... Rest>
        auto push(F && f, Rest&&... rest) {
            _pool.push(f, rest...);
        }

        template<typename F>
        void push(F&& f) {
            _pool.push(f);
        }

        void stop() {
            _pool.stop(false);
        }

    private:
        ctpl::thread_pool _pool;
    };
}