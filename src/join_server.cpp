#include <iostream>
#include <thread>

#include "server.h"

void do_service(boost::asio::io_service* svc);
void do_scale(int count, int port);

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: join_server <port>\n";
            return 1;
        }
        do_scale(2, std::atoi(argv[1]));
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

void do_service(boost::asio::io_service* svc)
{
    svc->run();
}

void do_scale(int count, int port)
{
    boost::asio::io_service io_service;
    server s(io_service, port);
    std::vector<std::thread> threads(count - 1);
    for (int i = 0; i < count - 1; i++)
        threads[i] = std::thread(do_service, &io_service);
    io_service.run();
    for (auto& t : threads)
        t.join();
}