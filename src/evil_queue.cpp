#include "evil_queue.h"

evil_queue::block_pair evil_queue::pop()
{
    std::unique_lock<std::mutex> guard(m_lock);
    if ((m_blocks.size() == 0) && (m_stop == false))
        m_cond.wait(guard);
    typename std::remove_reference<decltype(m_blocks.front())>::type ret;
    if (m_blocks.size() > 0)
    {
        ret = m_blocks.front();
        m_blocks.pop();
    }
    guard.unlock();
    return ret;
}

void evil_queue::push(block_shared block, const std::string& ts)
{
    std::lock_guard<std::mutex> guard(m_lock);
    m_blocks.push(std::make_pair(block, ts));
    m_cond.notify_one();
}

void evil_queue::stop()
{
    std::lock_guard<std::mutex> guard(m_lock);
    m_stop = true;
    m_cond.notify_all();
}
