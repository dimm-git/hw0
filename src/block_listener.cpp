#include <iostream>
#include <fstream>
#include <sstream>

#include "app_config.h"
#include "command_printer.h"

#include "block_listener.h"

block_logger::block_logger(block_printer* prn) : m_p(prn)
{
}

block_logger::~block_logger()
{
    done();
}

void block_logger::block_built(block_shared b)
{
    if (b)
        m_p->print(std::cout, *b);
}

void block_logger::block_break(block_shared)
{
}

void block_logger::command_accepted(command*)
{
}

void block_logger::command_rejected(command*)
{
}

void block_logger::done()
{
}

namespace
{

void print_func(evil_queue* q, block_printer* prn)
{
    do
    {
        auto data = q->pop();
        auto cb = data.first;
        if (!cb)
            break;
        std::ofstream mfs(data.second);
        prn->print(mfs, *cb);
    }
    while (true);
}

}

block_threaded_logger::block_threaded_logger(std::size_t count, block_printer* prn, logname_generator* gen) : m_gen(gen)
{
    for (std::size_t i = 0; i < count; i++)
        m_threads.push_back(std::thread(print_func, &m_queue, prn));
}

block_threaded_logger::~block_threaded_logger()
{
    done();
}

void block_threaded_logger::block_built(block_shared b)
{
    m_queue.push(b, m_gen->name());
}

void block_threaded_logger::block_break(block_shared)
{
    m_gen->name();
}

void block_threaded_logger::command_accepted(command*)
{
    m_gen->update_timestamp();
}

void block_threaded_logger::command_rejected(command*)
{
}

void block_threaded_logger::done()
{
    m_queue.stop();
    for (auto& t : m_threads)
        t.join();
    m_threads.clear();
}
