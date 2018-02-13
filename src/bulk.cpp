#include <cassert>
#include <cstdlib>
#include <iostream>

int main(int, const char**)
{
    try
    {
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
