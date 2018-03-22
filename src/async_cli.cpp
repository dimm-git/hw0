#include <iostream>
#include <thread>

#include "async.h"

void thread_f0(std::size_t bulk);
void thread_f1(std::size_t bulk);

int main(int, char *[])
{
    std::size_t bulk = 5;
    auto t0 = std::thread(thread_f0, bulk);
    auto t1 = std::thread(thread_f1, bulk);
    t0.join();
    t1.join();
    return 0;
}

void thread_f0(std::size_t bulk)
{
    auto h = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h, "\n2\n3\n4\n5\n6 7 8 9\n{\na\n", 21);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
}

void thread_f1(std::size_t bulk)
{
    auto h2 = async::connect(bulk);
    async::receive(h2, "1\n", 2);
    async::disconnect(h2);
}
