#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/time.h>

#include "command_printer.h"

#include "block_listener.h"

block_logger::block_logger(block_printer* prn) : m_p(prn)
{
}

void block_logger::block_built(command_block* b)
{
    m_p->print(std::cout, b);
}

void block_logger::block_break(command_block*)
{
}

void block_logger::command_accepted(command*)
{
}

void block_logger::command_rejected(command*)
{
}

block_lazy_logger::block_lazy_logger(block_printer* prn) : m_p(prn)
{
}


void block_lazy_logger::block_built(command_block* b)
{
    std::stringstream ss;
    ss << "bulk" << m_tv.tv_sec << "_" << m_tv.tv_usec << ".log";
    std::ofstream mfs(ss.str());
    m_p->print(mfs, b);
    m_tv = timeval { 0, 0 };
}

void block_lazy_logger::block_break(command_block*)
{
    m_tv = timeval { 0, 0 };
}

static bool operator==(const timeval& l, const timeval& r)
{
    return (l.tv_sec == r.tv_sec) && (l.tv_usec == r.tv_usec);
}

void block_lazy_logger::command_accepted(command*)
{
    if (m_tv == timeval { 0, 0 })
        gettimeofday(&m_tv, nullptr);
}

void block_lazy_logger::command_rejected(command*)
{
}
