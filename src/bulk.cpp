#include <iostream>

#include "app_config.h"
#include "input_handler.h"

void go(input_handler* h, std::istream& s);
void run();
void wait_all();

int main(int argc, const char* argv[])
{
    try
    {
        app_config::init(argc, argv);
        run();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void go(input_handler* h, std::istream& s)
{
    for (std::string line; std::getline(s, line) && h->next(line);)
        /* */;
    h->flush();
}

void run()
{
    app_config& i = app_config::instance();
    go(i.inp_handler.get(), i.get_mode().get_istream());
    wait_all();
}

void wait_all()
{
    app_config& i = app_config::instance();
    i.con_logger->done();
    i.file_logger->done();
}
