#pragma once

#include <string>

struct block_stats
{
    block_stats(std::string name_, bool extended = false) : name(name_), is_extended(extended)
    {
    }

    void inc_cmd(std::size_t count) { commands += count; }
    void inc_blk(std::size_t count) { blocks += count; }
    void inc_line(std::size_t count) { lines += count; }

    std::string name;

    std::size_t blocks = 0;
    std::size_t commands = 0;

    bool is_extended;
    std::size_t lines = 0;
};