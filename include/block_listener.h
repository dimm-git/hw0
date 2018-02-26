#pragma once

#include <thread>
#include <vector>

#include "evil_queue.h"

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

        virtual void done() = 0;
};

class block_printer;
class block_logger : public block_listener
{
    private:
        std::thread m_thread;
        evil_queue m_queue;

    public:
        block_logger(block_printer* prn);
        virtual ~block_logger() noexcept;

        void block_built(block_shared b) override;
        void block_break(block_shared b) override;

        void command_accepted(command* c) override;
        void command_rejected(command* c) override;

        void done() override;
};

class block_threaded_logger : public block_listener
{
    private:
        std::vector<std::thread> m_threads;
        logname_generator* m_gen = nullptr;
        evil_queue m_queue;

    public:
        block_threaded_logger(std::size_t count, block_printer* prn, logname_generator* gen);
        virtual ~block_threaded_logger() noexcept;

        void block_built(block_shared b) override;
        void block_break(block_shared b) override;

        void command_accepted(command* c) override;
        void command_rejected(command* c) override;

        void done() override;
};
