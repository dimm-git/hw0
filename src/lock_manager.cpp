#include <algorithm>
#include <functional>

#include "lock_manager.h"

namespace
{

table_list::iterator rec_lock(table_list::iterator b, table_list::iterator e, std::function<bool(std::shared_timed_mutex&)>& lock, std::function<void(std::shared_timed_mutex&)>& unlock)
{
    table_list::iterator i = b;
    for (; b != e; ++b)
    {
        auto& l = (*b)->get_lock();
        if (lock(l) == false)
            break; 
    }
    if (b != e)
    {
        for (; i != b; ++i)
        {
            auto& l = (*b)->get_lock();
            unlock(l);
        }   
    }
    return b;
}

void rec_unlock(table_list::iterator b, table_list::iterator e, std::function<void(std::shared_timed_mutex&)>& ul)
{
    auto f = [ &ul ](table* t) { ul(t->get_lock()); };
    std::for_each(b, e, f);
}

}

void do_lock(std::mutex& m, std::condition_variable& var, table_list& tabs, std::function<bool(std::shared_timed_mutex&)>& lock, std::function<void(std::shared_timed_mutex&)>& unlock)
{
    std::unique_lock<std::mutex> guard(m);
    do
    {
        table_list::iterator j = rec_lock(tabs.begin(), tabs.end(), lock, unlock);
        if (j == tabs.end())
            return;
        var.wait(guard);
    }
    while (true);
}

void lock_manager::shared_lock(table_list& tabs)
{
    std::function<bool(std::shared_timed_mutex&)> lock = &std::shared_timed_mutex::try_lock_shared;
    std::function<void(std::shared_timed_mutex&)> unlock = &std::shared_timed_mutex::unlock_shared;
    do_lock(m_lock, m_signal, tabs, lock, unlock);
}

void lock_manager::exclusive_lock(table_list& tabs)
{
    std::function<bool(std::shared_timed_mutex&)> lock = &std::shared_timed_mutex::try_lock;
    std::function<void(std::shared_timed_mutex&)> unlock = &std::shared_timed_mutex::unlock;
    do_lock(m_lock, m_signal, tabs, lock, unlock);
}

void lock_manager::shared_unlock(table_list& tabs)
{
    std::unique_lock<std::mutex> guard(m_lock);
    std::function<void(std::shared_timed_mutex&)> unlock = &std::shared_timed_mutex::unlock_shared;
    rec_unlock(tabs.begin(), tabs.end(), unlock);
    m_signal.notify_all();
}

void lock_manager::exclusive_unlock(table_list& tabs)
{
    std::unique_lock<std::mutex> guard(m_lock);
    std::function<void(std::shared_timed_mutex&)> unlock = &std::shared_timed_mutex::unlock;
    rec_unlock(tabs.begin(), tabs.end(), unlock);
    m_signal.notify_all();
}
