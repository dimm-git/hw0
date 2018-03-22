#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
#include <mutex>
#include <sstream>

#include "async_config.h"
#include "command_printer.h"

#include "block_listener.h"

namespace
{

void console_printer(block_printer* prn, evil_queue* q, block_stats* stats)
{
    static std::mutex console_lock;

    do
    {
        auto data = q->pop();
        auto cb = data.first;
        if (!cb)
            break;
        std::size_t cmd = 0;
        {
            std::lock_guard<std::mutex> guard(console_lock);
            cmd = prn->print(std::cout, *cb);
        }
        stats->inc_blk(1);
        stats->inc_cmd(cmd);
    }
    while (true);
}

void print_func(evil_queue* q, block_printer* prn, block_stats* stats)
{
    do
    {
        auto data = q->pop();
        auto cb = data.first;
        if (!cb)
            break;
        stats->inc_blk(1);
        std::ofstream mfs(data.second);
        std::size_t cmd = prn->print(mfs, *cb);
        stats->inc_cmd(cmd);
    }
    while (true);
}

}

block_logger::block_logger(block_printer* prn, std::size_t ql) : m_queue(ql)
{
    m_stats.push_back(block_stats("log"));
    m_thread = std::thread(console_printer, prn, &m_queue, &m_stats[0]);
}

block_logger::~block_logger()
{
    done();
}

void block_logger::block_built(block_shared b)
{
    m_queue.push(b, "");
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
    if (m_thread.joinable() == true)
    {
        m_queue.stop();
        m_thread.join();
    }
}

const std::vector<block_stats>& block_logger::get_stats()
{
    return m_stats;
}

block_threaded_logger::block_threaded_logger(std::size_t count, block_printer* prn, logname_generator* gen, std::size_t ql) : m_gen(gen), m_queue(ql)
{
    m_stats.reserve(count);
    for (std::size_t i = 0; i < count; i++)
    {
        std::stringstream ss;
        ss << "file" << i;
        m_stats.push_back(block_stats(ss.str()));
        m_threads.push_back(std::thread(print_func, &m_queue, prn, &m_stats[i]));
    }
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

const std::vector<block_stats>& block_threaded_logger::get_stats()
{
    return m_stats;
}

total_stats::total_stats()
{
    m_stats.push_back(block_stats("main", true));
}

void total_stats::block_built(block_shared)
{
    m_stats[0].inc_blk(1);
    m_stats[0].inc_cmd(block_commands);
    block_commands = 0;
}

void total_stats::block_break(block_shared)
{
    block_commands = 0;
}

void total_stats::command_accepted(command*)
{
    block_commands++;
}

void total_stats::command_rejected(command*)
{
}

void total_stats::done()
{
}

const std::vector<block_stats>& total_stats::get_stats()
{
    return m_stats;
}

void total_stats::line_read()
{
    m_stats[0].inc_line(1);
}
