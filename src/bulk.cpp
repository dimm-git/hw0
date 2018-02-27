#include <iostream>

#include "app_config.h"
#include "input_handler.h"

void go(input_handler* h, std::istream& s);
void run();
void wait_all();
void print_app_stats();
void print_thread_stats(const std::vector<block_stats>& s);
void print_stats(const block_stats& s);

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
    total_stats* stat = app_config::instance().app_stats.get();

    for (std::string line; std::getline(s, line) && h->next(line);)
        stat->line_read();

    h->flush();
}

void run()
{
    app_config& i = app_config::instance();
    go(i.inp_handler.get(), i.get_mode().get_istream());
    wait_all();
    print_app_stats();
}

void wait_all()
{
    app_config& i = app_config::instance();
    i.con_logger->done();
    i.file_logger->done();
}

void print_app_stats()
{
    app_config& i = app_config::instance();
    std::cout << std::endl << "Application statistics by thread:" << std::endl;
    print_thread_stats(i.con_logger->get_stats());
    print_thread_stats(i.file_logger->get_stats());
    print_thread_stats(i.app_stats->get_stats());
}

void print_thread_stats(const std::vector<block_stats>& st)
{
    for (auto s : st)
        print_stats(s);
}

void print_stats(const block_stats& s)
{
    std::cout << s.name << ":" << std::endl;
    if (s.is_extended == true)
    {
        std::cout << "\tlines: " << s.lines << std::endl;
        std::cout << "\tcommands: " << s.commands << std::endl;
        std::cout << "\tblocks: " << s.blocks << std::endl;
    }
    else
    {
        std::cout << "\tblocks: " << s.blocks << std::endl;
        std::cout << "\tcommands: " << s.commands << std::endl;
    }
}
