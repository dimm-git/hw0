#include <algorithm>
#include <atomic>
#include <iterator>
#include <memory>
#include <mutex>
#include <sstream>
#include <stack>

#include "async_config.h"

#include "async.h"

namespace
{

class the_ctype : public std::ctype<char>
{
    private:
        mask the_table[table_size];

    public:
        the_ctype(size_t refs = 0) : std::ctype<char>(&the_table[0], false, refs)
        {
            std::copy_n(classic_table(), table_size, the_table);
            auto pred = [ ](decltype(the_table[0]) val) { return (val & ctype_base::space) == ctype_base::space; };
            for (std::size_t i = 0; i < table_size; i++)
            {
                if (pred(the_table[i]) == true)
                    the_table[i] &= ~ctype_base::space;
            }
            the_table['\n'] |= (mask)space;
        }
};

struct rit
{
    typedef const char* value_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    typedef const char* pointer;
    typedef const char*& reference;

    const char* ptr = 0;

    explicit rit(const char* i) : ptr(i) { }

    rit& operator++()
    {
        --ptr;
        return *this;
    }

    operator const char*()
    {
        return ptr;
    }
};

class bulk_adapter
{
    private:
        std::unique_ptr<async_config> config;
        std::stringstream buf;

    public:
        bulk_adapter(std::size_t bulk) : config(new async_config(bulk))
        {
            std::locale x(std::locale::classic(), new the_ctype);
            buf.imbue(x);
        }

        void process(const char* data, std::size_t size)
        {
            if (size == 0)
                return;

            auto dbeg = rit(data + size - 1);
            auto dend = rit(data - 1);

            auto pos = std::find(dbeg, dend, '\n');

            if (dend != pos)
            {
                std::size_t lp = pos - data;
                buf.write(data, lp);
                std::istream_iterator<std::string> ss{buf};
                std::istream_iterator<std::string> se;
                total_stats* ts = config->app_stats.get();
                input_handler* h = config->inp_handler.get();
                auto proc = [ ts, h ](std::string line)
                {
                    h->next(line);
                    ts->line_read();
                };
                std::for_each(ss, se, proc);
                data = pos;
                size = dbeg - pos;
            }
            buf.clear();
            if (size != 0)
                buf.write(data, size);
        }

        void done()
        {
            if (config->inp_handler.is_null() == false)
                config->inp_handler->flush();
            if (config->con_logger.is_null() == false)
                config->con_logger->done();
            if (config->file_logger.is_null() == false)
                config->file_logger->done();
        }
};

}

struct async_handle
{
    std::mutex lock;
    std::unique_ptr<bulk_adapter> adapter;
    std::atomic<int> g; // generation

    async_handle(std::size_t bulk) : adapter(new bulk_adapter(bulk))
    {
    }

    void process(const char* data, std::size_t size)
    {
        std::atomic<int> g0;
        g0.store(g.load());
        std::unique_lock<std::mutex> lk(lock);
        if ((g0 != g) || (!adapter))
            return;
        adapter->process(data, size);
    }

    void done()
    {
        std::atomic<int> g0;
        g0.store(g.load());
        std::unique_lock<std::mutex> lk(lock);
        if ((g0 != g) || (!adapter))
            return;
        adapter->done();
        adapter.reset(nullptr);
        g = g + 1;
    }

    void reset(std::size_t bulk)
    {
        adapter.reset(new bulk_adapter(bulk));
        g = g + 1;
    }
};

namespace async
{

static std::mutex pool_lock;
static std::stack<std::unique_ptr<async_handle> > pool;

handle_t connect(std::size_t bulk)
{
    std::unique_lock<std::mutex> lock(pool_lock);
    if (pool.empty())
        return new async_handle(bulk);
    std::unique_ptr<async_handle> h(std::move(pool.top()));
    pool.pop();
    h->reset(bulk);
    return h.release();
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    async_handle* h = reinterpret_cast<async_handle*>(handle);
    h->process(data, size);
}

void disconnect(handle_t handle)
{
    async_handle* h = reinterpret_cast<async_handle*>(handle);
    std::unique_lock<std::mutex> lock(pool_lock);
    std::unique_ptr<async_handle> guard(h);
    guard->done();
    pool.push(std::move(guard));
}

}
