/// \file

#include <iostream>

#include <vector>
#include <list>

#include "print_ip.h"

int main(int, char**)
{
    try
    {
        print_ip(std::cout, char(-1));
        std::cout << std::endl;
        print_ip(std::cout, short(0));
        std::cout << std::endl;
        print_ip(std::cout, int(2130706433));
        std::cout << std::endl;
        print_ip(std::cout, long(8875824491850138409));
        std::cout << std::endl;
        //!!!!!
        print_ip(std::cout, std::string("ip.from.string"));
        std::cout << std::endl;
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        print_ip(std::cout, v);
        std::cout << std::endl;

        std::list<int> l = { 6, 7, 8, 9, 10 };
        print_ip(std::cout, l);
        std::cout << std::endl;

        print_ip(std::cout, std::make_tuple(11, 12, 13, 14, 15, 16));
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
