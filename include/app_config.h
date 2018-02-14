#pragma once

#include <cstdlib>
#include <memory>

#include "app_mode.h"
#include "block_builder.h"
#include "block_listener.h"
#include "command_factory.h"
#include "command_printer.h"
#include "input_handler.h"

class app_config
{
    private:
        static std::unique_ptr<app_mode> mode;
        static std::unique_ptr<app_config> inst;

        std::unique_ptr<build_state> bstate_plain;
        std::unique_ptr<build_state> bstate_nested;
        std::unique_ptr<build_state> bstate;
        std::unique_ptr<command_block> cmdblock;
        std::unique_ptr<command_factory> factory;
        std::unique_ptr<block_builder> bbuilder;
        std::unique_ptr<input_handler> inh;

        std::unique_ptr<command_printer> cmdp;
        std::unique_ptr<block_printer> bp;

        std::unique_ptr<block_listener> log0;
        std::unique_ptr<block_listener> log1;

    public:
        static void init(int argc, const char* argv[]);
        static app_config& instance();

        app_mode& get_mode()
        {
            return *mode.get();
        }

        build_state* get_build_state()
        {
            if (bstate.get() == nullptr)
            {
                bstate_plain.reset(new build_state_plain(instance().get_mode().get_block_size()));
                bstate_nested.reset(new build_state_nested());
                bstate.reset(new build_state_mixed(bstate_plain.get(), bstate_nested.get()));
            }
            return bstate.get();
        }

        command_factory* get_command_factory()
        {
            if (factory.get() == nullptr)
                factory.reset(new input_command_factory);
            return factory.get();
        }

        command_block* get_command_block()
        {
            if (cmdblock.get() == nullptr)
                cmdblock.reset(new command_block);
            return cmdblock.get();
        }

        block_builder* get_block_builder()
        {
            if (bbuilder.get() == nullptr)
            {
                bbuilder.reset(new block_builder(get_build_state(), get_command_block()));
                bbuilder->add_listener(get_con_logger());
                bbuilder->add_listener(get_file_logger());
            }

            return bbuilder.get();
        }

        input_handler* get_input_handler()
        {
            if (inh.get() == nullptr)
                inh.reset(new command_block_handler(get_command_factory(), get_block_builder()));
            return inh.get();
        }

        command_printer* get_command_printer()
        {
            if (cmdp.get() == nullptr)
                cmdp.reset(new command_printer_name);

            return cmdp.get();
        }

        block_printer* get_block_printer()
        {
            if (bp.get() == nullptr)
                bp.reset(new block_printer_bulk(*get_command_printer()));
            return bp.get();
        }

        block_listener* get_con_logger()
        {
            if (log0.get() == nullptr)
                log0.reset(new block_logger(get_block_printer()));
            return log0.get();
        }

        block_listener* get_file_logger()
        {
            if (log1.get() == nullptr)
                log1.reset(new block_lazy_logger(get_block_printer(), &instance().get_mode().get_generator()));
            return log1.get();
        }
};
