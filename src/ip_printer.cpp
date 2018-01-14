#include <iostream>

#include <vector>
#include <list>

#include "print_ip.h"

int main(int, char**)
{
    try
    {
        print_ip(std::cout, 0x00FF00FE);
        std::cout << std::endl;
        print_ip(std::cout, uint16_t(5));
        std::cout << std::endl;
        print_ip(std::cout, int8_t(6));
        std::cout << std::endl;
        print_ip(std::cout, std::make_tuple(1, 1));
        std::cout << std::endl;
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        print_ip(std::cout, v);
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
