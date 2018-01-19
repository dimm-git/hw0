/// \file

#include <iostream>

#include <vector>
#include <list>

#include "print_ip.h"

int main(int, char**)
{
    try
    {
        print_ip(std::cout, char(-1)) << std::endl;
        print_ip(std::cout, short(0)) << std::endl;
        print_ip(std::cout, int(2130706433)) << std::endl;
        print_ip(std::cout, long(8875824491850138409)) << std::endl;
        print_ip(std::cout, std::string("ip.from.string")) << std::endl;
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        print_ip(std::cout, v) << std::endl;

        std::list<std::string> l = { "6", "7", "8", "9", "10" };
        print_ip(std::cout, l) << std::endl;
        print_ip(std::cout, std::make_tuple(11, 12, 13, 14, 15, 16)) << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
