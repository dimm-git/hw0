#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "block_builder.h"
#include "block_listener.h"
#include "command_factory.h"
#include "command_printer.h"
#include "input_handler.h"

void go(input_handler* h, std::istream& s);
void run(int depth);

#define DEBUG

int main(int argc, const char* argv[])
{
    try
    {
#ifndef DEBUG
        if (argc != 2)
            throw std::runtime_error("usage: buld <block length>");
        int depth = 0;
        std::stringstream ss;
        ss << argv[1];
        ss >> depth;
#else
        int depth = 3;
#endif
        run(depth);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void go(input_handler* h, std::istream& s)
{
    for (std::string line; std::getline(s, line) && h->next(line);)
        /* */;
    h->flush();
}

void run(int depth)
{
    std::unique_ptr<build_state> bstate_plain(new build_state_plain(depth));
    std::unique_ptr<build_state> bstate_nested(new build_state_nested());
    std::unique_ptr<build_state> bstate(new build_state_mixed(bstate_plain.get(), bstate_nested.get()));

    std::unique_ptr<command_block> cmdblock(new command_block);

    std::unique_ptr<command_factory> factory(new input_command_factory);
    std::unique_ptr<block_builder> bbuilder(new block_builder(bstate.get(), cmdblock.get()));

    std::unique_ptr<input_handler> inh(new command_block_handler(factory.get(), bbuilder.get()));

    std::unique_ptr<command_printer> cmdp(new command_printer_name);
    std::unique_ptr<block_printer> bp(new block_printer_bulk(*cmdp.get()));

    std::unique_ptr<block_listener> log0(new block_logger(bp.get()));
    std::unique_ptr<block_listener> log1(new block_lazy_logger(bp.get()));
    bbuilder->add_listener(log0.get());
    bbuilder->add_listener(log1.get());

#ifdef DEBUG
    std::ifstream strm("input.txt", std::ifstream::in);
    go(inh.get(), strm);
#else
    go(inh.get(), std::cin);
#endif
}
