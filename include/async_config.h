#pragma once

#include <functional>
#include <memory>

#include "block_builder.h"
#include "block_listener.h"
#include "command_factory.h"
#include "command_printer.h"
#include "input_handler.h"
#include "logname_generator.h"

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

        bool is_null() const
        {
            return val.get() == nullptr;
        }

        V* operator->()
        {
            return get();
        }

    private:
        T* m_p = nullptr;
        std::function<V*(T*)> m_f;
        std::unique_ptr<V> val;
};

class async_config
{
    private:
        std::unique_ptr<logname_generator> gen;
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
        total_stats* make_app_stats();
        onion_block_printer* make_fake_work_printer();

        std::size_t block_size = 3;
        std::size_t queue_length = 1;
        std::size_t fakebuf_size = 10000;
        std::size_t fakebuf_count = 100;

    private:
        void init();

    public:
        async_config(std::size_t bulk = 3);

        virtual std::size_t get_block_size() const noexcept;
        virtual logname_generator& get_generator() const noexcept;
        virtual std::size_t get_queue_length() const noexcept;

        virtual std::size_t get_fakebuf_size() const noexcept;
        virtual std::size_t get_fakework_count() const noexcept;

        lazy_init<async_config, build_state> build_st;
        lazy_init<async_config, command_factory> cmd_factory;
        lazy_init<async_config, input_handler> inp_handler;
        lazy_init<async_config, block_builder> bbuilder;
        lazy_init<async_config, command_printer> cmd_printer;
        lazy_init<async_config, block_printer> blk_printer;
        lazy_init<async_config, block_listener> con_logger;
        lazy_init<async_config, block_listener> file_logger;
        lazy_init<async_config, total_stats> app_stats;
        lazy_init<async_config, onion_block_printer> fakework_printer;
};
