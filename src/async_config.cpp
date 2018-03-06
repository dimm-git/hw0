#include <sstream>

#include "async_config.h"

namespace
{
template <typename V1, typename V2>
void process_vars(const V1& f1, V2& f2)
{
    for (std::size_t i = 0; i < f1.size(); i++)
    {
        const char* val = std::getenv(f1[i].c_str());
        if (val != nullptr)
        {
            std::stringstream ss;
            ss << val;
            ss >> *f2[i];
        }
    }
}

}

void async_config::init()
{
    // :KLUDGE:
    static std::array<std::string, 5> env = { "ASYNC_QUEUE_LEN", "ASYNC_FBS", "ASYNC_FBC" };
    std::array<decltype(&this->block_size), 5> fields = { &queue_length, &fakebuf_size, &fakebuf_count };
    process_vars(env, fields);
}

command_factory* async_config::make_command_factory()
{
    return new input_command_factory;
}

input_handler* async_config::make_input_handler()
{
    return new command_block_handler(cmd_factory.get(), bbuilder.get());
}

build_state* async_config::make_build_state()
{
    bstate_plain.reset(new build_state_plain(get_block_size()));
    bstate_nested.reset(new build_state_nested());
    return new build_state_mixed(bstate_plain.get(), bstate_nested.get());
}

 block_builder* async_config::make_block_builder()
 {
    std::unique_ptr<block_builder> bbuilder(new block_builder(build_st.get()));
    bbuilder->add_listener(con_logger.get());
    bbuilder->add_listener(file_logger.get());
    bbuilder->add_listener(app_stats.get());

    return bbuilder.release();
}

command_printer* async_config::make_command_printer()
{
    return new command_printer_name;
}

block_printer* async_config::make_block_printer()
{
    return new block_printer_bulk(*cmd_printer.get());
}

block_listener* async_config::make_con_logger()
{
    return new block_logger(fakework_printer.get(), get_queue_length());
}

block_listener* async_config::make_file_logger()
{
    return new block_threaded_logger(2, fakework_printer.get(), &get_generator(), get_queue_length());
}

total_stats* async_config::make_app_stats()
{
    return new total_stats;
}

onion_block_printer* async_config::make_fake_work_printer()
{
    return new onion_block_printer(blk_printer.get(), get_fakebuf_size(), get_fakework_count());
}

async_config::async_config(std::size_t bulk) :
    gen(new logname_generator_ms),
    block_size(bulk),
    build_st(this, &async_config::make_build_state),
    cmd_factory(this, &async_config::make_command_factory),
    inp_handler(this, &async_config::make_input_handler),
    bbuilder(this, &async_config::make_block_builder),
    cmd_printer(this, &async_config::make_command_printer),
    blk_printer(this, &async_config::make_block_printer),
    con_logger(this, &async_config::make_con_logger),
    file_logger(this, &async_config::make_file_logger),
    app_stats(this, &async_config::make_app_stats),
    fakework_printer(this, &async_config::make_fake_work_printer)

{
    init();
}

std::size_t async_config::get_block_size() const noexcept
{
    return block_size;
}

logname_generator& async_config::get_generator() const noexcept
{
    return *gen.get();
}

std::size_t async_config::get_queue_length() const noexcept
{
    return queue_length;
}

std::size_t async_config::get_fakebuf_size() const noexcept
{
    return fakebuf_size;
}

std::size_t async_config::get_fakework_count() const noexcept
{
    return fakebuf_count;
}
