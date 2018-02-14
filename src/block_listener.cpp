#include <iostream>
#include <fstream>
#include <sstream>

#include "app_config.h"
#include "command_printer.h"

#include "block_listener.h"

block_logger::block_logger(block_printer* prn) : m_p(prn)
{
}

void block_logger::block_built(command_block* b)
{
    m_p->print(std::cout, *b);
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

block_lazy_logger::block_lazy_logger(block_printer* prn, logname_generator* gen) : m_p(prn), m_gen(gen)
{
}


void block_lazy_logger::block_built(command_block* b)
{
    std::ofstream mfs(m_gen->name());
    m_p->print(mfs, *b);
}

void block_lazy_logger::block_break(command_block*)
{
    m_gen->name();
}

void block_lazy_logger::command_accepted(command*)
{
    m_gen->update_timestamp();
}

void block_lazy_logger::command_rejected(command*)
{
}