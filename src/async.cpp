#include <memory>
#include <mutex>

#include "async_config.h"

#include "async.h"

struct async_handle
{
    std::unique_ptr<async_config> config;
    std::unique_ptr<std::mutex> lock;

    async_handle(std::size_t bulk) : config(new async_config(bulk)), lock(new std::mutex)
    {
    }

    std::mutex& l()
    {
        return *lock.get();
    }

    ~async_handle() noexcept
    {
    }
};

namespace async
{

handle_t connect(std::size_t bulk)
{
    return new async_handle(bulk);
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    async_handle* h = reinterpret_cast<async_handle*>(handle);
    std::unique_lock<std::mutex> l(h->l());
}

void disconnect(handle_t handle)
{
    async_handle* h = reinterpret_cast<async_handle*>(handle);
    std::unique_lock<std::mutex> l(h->l());
    std::unique_ptr<std::mutex> cur(std::move(h->lock));
    delete h;
}

}
