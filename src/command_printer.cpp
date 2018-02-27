#include <algorithm>

#include "command_printer.h"

void command_printer_name::print(std::ostream& s, const command& cmd)
{
    s << cmd.name();
}

block_printer_bulk::block_printer_bulk(command_printer& p) : m_cmdp(p)
{
}

std::size_t block_printer_bulk::print(std::ostream& s, const command_block& block)
{
    std::size_t count = 0;
    command_printer& p = m_cmdp;
    auto f = [ &s, &p, &count ](decltype(*block.begin())& cmd) { s << " "; p.print(s, *cmd.get()); count++; };
    s << "bulk:";
    std::for_each(block.begin(), block.end(), f);
    s << std::endl;
    return count;
}
