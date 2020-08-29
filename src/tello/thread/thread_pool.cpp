#include "thread_pool.hpp"
#include <tello/native/network_interface.hpp>
#include "thread_pool_impl.hpp"

using tello::NetworkResponse;

using tello::threading::ThreadPoolImpl;

tello::threading::Threadpool::Threadpool() : _impl(new ThreadPoolImpl()) {}

void tello::threading::Threadpool::push(std::function<void(int)>&& function) {
    _impl->push(function);
}

void tello::threading::Threadpool::stop() {
    _impl->stop();
}

tello::threading::Threadpool::~Threadpool() = default;
