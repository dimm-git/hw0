#include <algorithm>

#include "command_printer.h"

void command_printer_name::print(std::ostream& s, const command* cmd)
{
    s << cmd->name();
}

block_printer_bulk::block_printer_bulk(command_printer& p) : m_cmdp(p)
{

}

void block_printer_bulk::print(std::ostream& s, const command_block* block)
{
    command_printer& p = m_cmdp;
    auto f = [ &s, &p ](decltype(*block->begin())& cmd) { s << " "; p.print(s, cmd.get()); };
    s << "bulk:";
    std::for_each(block->begin(), block->end(), f);
    s << std::endl;
}
