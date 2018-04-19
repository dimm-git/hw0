#include <thread>

#include "reducer.h"

#include "reduce.h"

namespace
{
void reducef(std::list<std::string>& m, std::size_t i);
}

// TOD: make template
void make_reduce(maps& m)
{
    std::list<std::thread> t;
    for (std::size_t i = 0; i < m.size(); i++)
    {
        std::thread d(reducef, std::ref(m[i]), i);
        t.emplace_back(std::move(d));
    }
    for (auto& v : t)
        v.join();
}

namespace
{

void reducef(std::list<std::string>& l, std::size_t i)
{
    reducer r{};
    for (auto& v : l)
        r.reduce(v);
    auto x = r.result();
}

}