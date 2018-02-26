#pragma once

#include "block_shared.h"

class logname_generator;

class block_listener
{
    public:
        virtual ~block_listener() noexcept { }

        virtual void block_built(block_shared b) = 0;
        virtual void block_break(block_shared b) = 0;

        virtual void command_accepted(command* c) = 0;
        virtual void command_rejected(command* c) = 0;
};

class block_printer;
class block_logger : public block_listener
{
    private:
        block_printer* m_p = nullptr;

    public:
        block_logger(block_printer* prn);
        void block_built(block_shared b) override;
        void block_break(block_shared b) override;

        void command_accepted(command* c) override;
        void command_rejected(command* c) override;
};

class block_lazy_logger : public block_listener
{
    private:
        block_printer* m_p = nullptr;
        logname_generator* m_gen = nullptr;

    public:
        block_lazy_logger(block_printer* prn, logname_generator* gen);
        void block_built(block_shared b) override;
        void block_break(block_shared b) override;

        void command_accepted(command* c) override;
        void command_rejected(command* c) override;
};
