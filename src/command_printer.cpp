#include <algorithm>
#include <vector>

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
    auto f = [ &s, &p, &count ](decltype(*block.begin())& cmd)
    {
        input_command tmp(cmd);
        s << " "; p.print(s, tmp); count++;
    };
    s << "bulk:";
    std::for_each(block.begin(), block.end(), f);
    s << std::endl;
    return count;
}

namespace
{

void do_fake_work(std::vector<int>& tmp, std::size_t count)
{
    for (std::size_t i = 0; i < count; i++)
        std::generate(tmp.begin(), tmp.end(), std::rand);
}

}

onion_block_printer::onion_block_printer(block_printer* printer, std::size_t fbsize, std::size_t fbcount) : sibling(printer), size(fbsize), count(fbcount)
{
}

std::size_t onion_block_printer::print(std::ostream& s, const command_block& block)
{
    // :KLUDGE: make per-thread fake buffer
    std::vector<int> fakebuf(size);
    do_fake_work(fakebuf, count);
    return sibling->print(s, block);
}
