#pragma once

#include <functional>
#include <memory>

#include "app_mode.h"
#include "block_builder.h"
#include "block_listener.h"
#include "command_factory.h"
#include "command_printer.h"
#include "input_handler.h"

template <typename T, typename V>
class lazy_init
{
    public:
        lazy_init(T* parent, std::function<V*(T*)> func) : m_p(parent), m_f(func)
        {
        }

        V* get()
        {
            if (val.get() == nullptr)
                val.reset(m_f(m_p));
            return val.get();
        }

    private:
        T* m_p = nullptr;
        std::function<V*(T*)> m_f;
        std::unique_ptr<V> val;
};

class app_config
{
    private:
        static std::unique_ptr<app_mode> mode;
        static std::unique_ptr<app_config> inst;

        std::unique_ptr<build_state> bstate_plain;
        std::unique_ptr<build_state> bstate_nested;

        command_factory* make_command_factory();
        input_handler* make_input_handler();
        build_state* make_build_state();
        block_builder* make_block_builder();
        command_printer* make_command_printer();
        block_printer* make_block_printer();
        block_listener* make_con_logger();
        block_listener* make_file_logger();

    public:
        static void init(int argc, const char* argv[]);
        static app_config& instance();

        app_mode& get_mode()
        {
            return *mode.get();
        }

        app_config();

        lazy_init<app_config, build_state> build_st;
        lazy_init<app_config, command_factory> cmd_factory;
        lazy_init<app_config, input_handler> inp_handler;
        lazy_init<app_config, block_builder> bbuilder;
        lazy_init<app_config, command_printer> cmd_printer;
        lazy_init<app_config, block_printer> blk_printer;
        lazy_init<app_config, block_listener> con_logger;
        lazy_init<app_config, block_listener> file_logger;
};
