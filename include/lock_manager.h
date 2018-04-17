#pragma once

#include <condition_variable>

#include "operation.h"

class lock_manager
{
    private:
        std::mutex m_lock;
        std::condition_variable m_signal;

    public:
        void shared_lock(table_list& tabs);
        void shared_unlock(table_list& tabs);
        void exclusive_lock(table_list& tabs);
        void exclusive_unlock(table_list& tabs);
};

struct lock_manager_guard
{
    table_list& m_tabs;
    lock_manager& m_m;
    bool m_mod;

    lock_manager_guard(table_list& tabs, lock_manager& m, bool mod) : m_tabs(tabs), m_m(m), m_mod(mod)
    {
        if (m_mod == true)
            m_m.exclusive_lock(tabs);
        else
            m_m.shared_lock(m_tabs);
    }

    ~lock_manager_guard()
    {
        if (m_mod == true)
            m_m.exclusive_unlock(m_tabs);
        else
            m_m.shared_unlock(m_tabs);
    }
};
