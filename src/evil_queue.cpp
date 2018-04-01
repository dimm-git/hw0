#include "evil_queue.h"

evil_queue::evil_queue(std::size_t size) : m_size(size)
{
}

evil_queue::block_pair evil_queue::pop()
{
    std::unique_lock<std::mutex> lock(m_lock);

    if ((m_blocks.size() == 0) && (m_stop == false))
    {
        decltype(m_blocks)& blocks = m_blocks;
        bool& stop = m_stop;
        auto f = [ &blocks, &stop ]()
        {
            return ((stop == true) || (blocks.size() > 0));
        };
        m_cond_data.wait(lock,  f);
    }
    typename std::remove_reference<decltype(m_blocks.front())>::type ret;
    if (m_blocks.size() > 0)
    {
        ret = m_blocks.front();
        m_blocks.pop();
        m_cond_space.notify_all();
        lock.unlock();
    }
    else
    {
        lock.unlock();
    }
    return ret;
}

void evil_queue::push(block_shared block, const std::string& ts)
{
    std::unique_lock<std::mutex> lock(m_lock);
    if (m_blocks.size() >= m_size)
    {
        decltype(m_blocks)& blocks = m_blocks;
        bool& stop = m_stop;
        std::size_t ql = m_size;
        auto f = [ &blocks, &stop, ql ]()
        {
            return ((stop == true) || (blocks.size() < ql));
        };
        m_cond_space.wait(lock, f);
    }
    m_blocks.push(std::make_pair(block, ts));
    m_cond_data.notify_one();
    lock.unlock();
}

void evil_queue::stop()
{
    std::lock_guard<std::mutex> guard(m_lock);
    m_stop = true;
    m_cond_space.notify_all();
    m_cond_data.notify_all();
}
