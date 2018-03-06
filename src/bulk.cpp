#include <fstream>
#include <iostream>
#include <sstream>

#include "async_config.h"
#include "input_handler.h"

void go(total_stats* stats, input_handler* h, std::istream& s);
void run(async_config* cfg, std::istream& stream);
void wait_all(async_config* cfg);
void print_app_stats(async_config* cfg);
void print_thread_stats(const std::vector<block_stats>& s);
void print_stats(const block_stats& s);

int main(int argc, const char* argv[])
{
    try
    {
        std::istream* current = &std::cin;
        std::fstream fin;
        if (argc > 1)
        {
            fin.open(argv[1], std::ios::in);
            current = &fin;
        }
        std::unique_ptr<async_config> cfg(new async_config);
        run(cfg.get(), *current);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void go(total_stats* stat, input_handler* h, std::istream& s)
{
    for (std::string line; std::getline(s, line) && h->next(line);)
        stat->line_read();

    h->flush();
}

void run(async_config* cfg, std::istream& stream)
{
    go(cfg->app_stats.get(), cfg->inp_handler.get(), stream);
    wait_all(cfg);
    print_app_stats(cfg);
}

void wait_all(async_config* cfg)
{
    cfg->con_logger->done();
    cfg->file_logger->done();
}

void print_app_stats(async_config* cfg)
{
    std::cout << std::endl << "Application statistics by thread:" << std::endl;
    print_thread_stats(cfg->con_logger->get_stats());
    print_thread_stats(cfg->file_logger->get_stats());
    print_thread_stats(cfg->app_stats->get_stats());
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
