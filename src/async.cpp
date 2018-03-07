#include <algorithm>
#include <iterator>
#include <memory>
#include <mutex>
#include <sstream>

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
            std::replace(the_table, the_table + table_size, ctype_base::space, ctype_base::alpha);
            the_table['\n'] = (mask)space;
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

}

#include <iostream>

struct async_handle
{
    std::unique_ptr<async_config> config;
    // :KLUDGE: it must but not be a smart pointer?
    // :TODO: make SSHELL
    std::unique_ptr<std::mutex> lock;

    std::stringstream buf;

    async_handle(std::size_t bulk) : config(new async_config(bulk)), lock(new std::mutex)
    {
        std::locale x(std::locale::classic(), new the_ctype);
        buf.imbue(x);
    }

    std::mutex& l()
    {
        return *lock.get();
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
        if (size != 0)
        {
            buf.clear();
            buf.write(data, size);
        }
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
    h->process(data, size);
}

void disconnect(handle_t handle)
{
    // :KLUDGE: make SSHELL
    async_handle* h = reinterpret_cast<async_handle*>(handle);
    std::unique_ptr<std::mutex> tmp; // to release it after lock
    std::unique_lock<std::mutex> lock(h->l());
    tmp.reset(h->lock.release());
    h->done();
    delete h;
}

}
