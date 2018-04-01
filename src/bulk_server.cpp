#include <iostream>

#include "server.h"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: bulk_server <port> <bulk size>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        server s(io_service, std::atoi(argv[1]), std::atoi(argv[2]));

        io_service.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
