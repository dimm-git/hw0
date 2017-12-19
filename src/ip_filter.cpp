#include <cassert>
#include <cstdlib>
#include <iostream>

#include <fstream>

#include <functional>

#include "ip_tools.h"
#include "ip_filter.h"
#include "ip_read.h"

template <typename... Args> inline void hide_unused(Args&&...) { }

int main(int argc, char const *argv[])
{
    hide_unused(argc, argv);
    try
    {
        auto ins = std::ifstream("/home/dimm/devel/hw1/build/x");
        auto& outs = std::cout;

        auto ip_pool = std::move(read_pool(ins));
        auto do_print = [ & ] (const decltype(ip_pool)& pool)
        {
            print_pool(pool, outs);
            std::cout << "--- EOF ---" << std::endl;
        };
        std::vector<std::function<void ()> > v
        { 
            [ & ]() { do_print(ip_pool); },
            [ & ]() { ip_sort(ip_pool); do_print(ip_pool); },
            [ & ]() { do_print(filter(ip_pool, 1)); },
            [ & ]() { do_print(filter(ip_pool, 46, 70)); },
            [ & ]() { do_print(filter_any(ip_pool, 46)); }
        };
        for (auto f : v)
            f();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
