#pragma once

#include <ctime>

#include "command_block.h"

class block_listener
{
    public:
        virtual ~block_listener() noexcept { }

        virtual void block_built(command_block* b) = 0;
        virtual void block_break(command_block* b) = 0;

        virtual void command_accepted(command* c) = 0;
        virtual void command_rejected(command* c) = 0;
};

class block_printer;
class block_logger : public block_listener
{
    private:
        block_printer* m_p;

    public:
        block_logger(block_printer* prn);
        void block_built(command_block* b) override;
        void block_break(command_block* b) override;

        void command_accepted(command* c) override;
        void command_rejected(command* c) override;
};

class block_lazy_logger : public block_listener
{
    private:
        block_printer* m_p;
        struct timeval m_tv = timeval { 0, 0 };

    public:
        block_lazy_logger(block_printer* prn);
        void block_built(command_block* b) override;
        void block_break(command_block* b) override;

        void command_accepted(command* c) override;
        void command_rejected(command* c) override;
};
