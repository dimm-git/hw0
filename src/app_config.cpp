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
