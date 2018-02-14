#include "build_state.h"

build_state_plain::build_state_plain(int size) : m_max_size(size)
{
}

void build_state_plain::reset()
{
    m_cur_size = 0;
}

block_state build_state_plain::push()
{
    return (m_cur_size > 0) ? bsFinish : bsReject;
}

block_state build_state_plain::check(command*)
{
    if (m_cur_size >= m_max_size)
        return bsReject;
    ++m_cur_size;
    if (m_cur_size == m_max_size)
        return bsFinish;
    return bsAccept;
}

block_state build_state_plain::pop()
{
    return bsError;
}

block_state build_state_plain::flush()
{
    return (m_cur_size > 0) ? bsFinish : bsAccept;
}

void build_state_nested::reset()
{
    m_level = 0;
}

block_state build_state_nested::push()
{
    ++m_level;

    return bsAccept;
}

block_state build_state_nested::check(command*)
{
    return bsAccept;
}

block_state build_state_nested::pop()
{
    if (m_level == 0)
        return bsError;
    --m_level;
    if (m_level == 0)
        return bsFinish;
    return bsAccept;
}

block_state build_state_nested::flush()
{
    return (m_level == 0) ? bsFinish : bsReject;
}

build_state_mixed::build_state_mixed(build_state* plain, build_state* rec) : m_plain(plain), m_rec(rec), m_cur(plain)
{
}

void build_state_mixed::reset()
{
    m_plain->reset();
    m_rec->reset();
    m_cur = m_plain;
}

block_state build_state_mixed::push()
{
    block_state bs = m_cur->push();
    if (m_cur == m_plain)
    {
        m_plain->reset();
        if (bs != bsFinish)
            bs = m_rec->push();
        else
            m_rec->push();
        m_cur = m_rec;
        return bs;
    }
    return bs;
}

block_state build_state_mixed::check(command* cmd)
{
    return m_cur->check(cmd);
}

block_state build_state_mixed::pop()
{
    block_state s = m_cur->pop();
    if (s == bsFinish)
        m_cur = m_plain;
    return s;
}

block_state build_state_mixed::flush()
{
    return m_cur->flush();
}
