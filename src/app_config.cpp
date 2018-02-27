#include "app_config.h"

std::unique_ptr<app_mode> app_config::mode = nullptr;
std::unique_ptr<app_config> app_config::inst = nullptr;

void app_config::init(int argc, const char* argv[])
{
    const char* debug = std::getenv("BULK_DEBUG");
    if (debug == nullptr || std::string(debug) != std::string("YES"))
        mode.reset(new app_mode_prod);
    else
        mode.reset(new app_mode_debug);
    mode->parse(argc, argv);
    inst.reset(new app_config);
}

app_config& app_config::instance()
{
    if (inst.get() == nullptr)
        throw std::runtime_error("App is not initialized properly");

    return *inst.get();
}

command_factory* app_config::make_command_factory()
{
    return new input_command_factory;
}

input_handler* app_config::make_input_handler()
{
    return new command_block_handler(cmd_factory.get(), bbuilder.get());
}

build_state* app_config::make_build_state()
{
    bstate_plain.reset(new build_state_plain(instance().get_mode().get_block_size()));
    bstate_nested.reset(new build_state_nested());
    return new build_state_mixed(bstate_plain.get(), bstate_nested.get());
}

 block_builder* app_config::make_block_builder()
 {
    std::unique_ptr<block_builder> bbuilder(new block_builder(build_st.get()));
    bbuilder->add_listener(con_logger.get());
    bbuilder->add_listener(file_logger.get());
    bbuilder->add_listener(app_stats.get());

    return bbuilder.release();
}

command_printer* app_config::make_command_printer()
{
    return new command_printer_name;
}

block_printer* app_config::make_block_printer()
{
    return new block_printer_bulk(*cmd_printer.get());
}

block_listener* app_config::make_con_logger()
{
    return new block_logger(blk_printer.get());
}

block_listener* app_config::make_file_logger()
{
    return new block_threaded_logger(2, blk_printer.get(), &instance().get_mode().get_generator());
}

total_stats* app_config::make_app_stats()
{
    return new total_stats;
}

app_config::app_config() :
    build_st(this, &app_config::make_build_state),
    cmd_factory(this, &app_config::make_command_factory),
    inp_handler(this, &app_config::make_input_handler),
    bbuilder(this, &app_config::make_block_builder),
    cmd_printer(this, &app_config::make_command_printer),
    blk_printer(this, &app_config::make_block_printer),
    con_logger(this, &app_config::make_con_logger),
    file_logger(this, &app_config::make_file_logger),
    app_stats(this, &app_config::make_app_stats)
{
}
