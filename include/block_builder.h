#pragma once

#include "command_block.h"

class block_builder
{
    private:
        command_block* m_block = nullptr;

        const int m_block_size = 0;
        int m_current_block_size = 0;

        int m_nest_level = 0;

    public:
        block_builder(int count, command_block* block) : m_block(block), m_block_size(count)
        {
        }
};
