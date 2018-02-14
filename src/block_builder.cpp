#include "block_builder.h"

block_builder::block_builder(build_state* bs, command_block* cb) : m_bs(bs), m_cb(cb)
{
}

block_builder::~block_builder() noexcept
{
}

void block_builder::add_listener(block_listener* bl)
{
    m_listeners.push_back(bl);
}

bool block_builder::command_push()
{
    block_state bs = m_bs->push();

    if (bs == bsFinish)
        do_finish();

    if (bs == bsAccept || bs == bsFinish)
        return true;

    do_break();

    return false;
}

bool block_builder::command_add(command* cmd)
{
    bool res = false;
    block_state bs = m_bs->check(cmd);
    if (bs == bsFinish || bs == bsAccept)
    {
        m_cb->add(cmd);
        notify_cmd_add(cmd);
        if (bs == bsFinish)
            do_finish();
        res = true;
    }
    else
    {
        notify_cmd_rej(cmd);
        do_break();
    }
    return res;
}

bool block_builder::command_pop()
{
    block_state bs = m_bs->pop();
    if (bs == bsFinish)
        do_finish();
    else if (bs != bsAccept)
        do_break();

    return (bs == bsFinish) || (bs == bsAccept);
}

bool block_builder::flush()
{
    block_state bs = m_bs->flush();
    if (bs == bsFinish)
        do_finish();
    else if (bs != bsAccept)
        do_break();
    return (bs == bsFinish) || (bs == bsAccept);
}

void block_builder::do_finish()
{
    notify_finish();
    reset();
}

void block_builder::notify_finish()
{
    for (l : m_listeners)
        l->block_built(m_cb);
}

void block_builder::do_break()
{
    notify_break();
    reset();
}

void block_builder::notify_break()
{
    for (l : m_listeners)
        l->block_break(m_cb);
}

void block_builder::notify_cmd_add(command* cmd)
{
    for (l : m_listeners)
        l->command_accepted(cmd);
}

void block_builder::notify_cmd_rej(command* cmd)
{
    for (l : m_listeners)
        l->command_rejected(cmd);
}

void block_builder::reset()
{
    m_cb->clear();
    m_bs->reset();
}
