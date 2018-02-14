#pragma once

#include "command_block.h"

class block_listener
{
    public:
        virtual ~block_listener() noexcept {}

        virtual void block_built(command_block* b) = 0;
        virtual void command_append(command* c) = 0;
};
