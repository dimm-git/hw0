#include <iostream>

#include "include/otus0.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
        std::cout << "Hello, World!" << std::endl;
    else if (std::string(argv[1]) == "-v")
        std::cout <<  "Version:" << " " << _VERSION_MAJOR << "." << _VERSION_MINOR << "." << _VERSION_PATCH << std::endl;
    return 0;
}
