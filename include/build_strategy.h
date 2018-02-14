#pragma once

#include "command.h"

enum block_state
{
    bsAccept,
    bsReject,
    bsFinish
};

class build_strategy
{
    public:
        virtual ~build_strategy() noexcept { }
        virtual block_state add(command* cmd) = 0;
};
