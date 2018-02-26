#pragma once

#include "block_listener.h"
#include "build_state.h"
#include "command.h"
#include "command_block.h"

class block_builder
{
    private:
        build_state* m_bs = nullptr;
        std::unique_ptr<command_block> m_cb;
        std::list<block_listener*> m_listeners;

    public:
        block_builder(build_state* bs);
        ~block_builder() noexcept;

        bool command_push();
        bool command_add(command* cmd);
        bool command_pop();
        bool flush();

        void add_listener(block_listener* bl);


    private:
        void reset();
        void do_finish();
        void notify_finish();
        void do_break();
        void notify_break();
        void notify_cmd_add(command* cmd);
        void notify_cmd_rej(command* cmd);
};
