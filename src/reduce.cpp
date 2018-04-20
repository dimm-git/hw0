#include <fstream>
#include <sstream>
#include <thread>

#include "reducer.h"

#include "reduce.h"

namespace
{
void reducef(std::list<std::string>& m, results& p);
void writef(results& p, std::size_t i);
}

// TOD: make template
void make_reduce(std::vector<results>& r, maps& m)
{
    std::list<std::thread> t;
    r.resize(m.size());
    for (std::size_t i = 0; i < m.size(); i++)
    {
        std::thread d(reducef, std::ref(m[i]), std::ref(r[i]));
        t.emplace_back(std::move(d));
    }
    for (auto& v : t)
        v.join();
 
    t.clear();
    for (std::size_t i = 0; i < m.size(); i++)
    {
        std::thread d(writef, std::ref(r[i]), i);
        t.emplace_back(std::move(d));
    }

    for (auto& v : t)
        v.join();
 }

namespace
{

void reducef(std::list<std::string>& l, results& p)
{
    reducer r{};
    for (auto& v : l)
        r.reduce(v);
    p = std::move(r.result());
}

void writef(results& p, std::size_t i)
{
    std::stringstream ss;
    ss << i << ".txt";
    std::ofstream f(ss.str().c_str());
    f << "#line,prefix,count" << std::endl;
    for (auto& v : p)
        f << std::get<0>(v) << "," << std::get<1>(v) << "," << std::get<2>(v) << std::endl;
}

}
