#include <fstream>
#include <thread>

#include "mapper.h"
#include "split.h"

#include "mapping.h"

namespace
{
void split(offsets& of, const std::string& src, int count);
void map(maps& m, const std::string& src, offsets& of);
}

void make_map(maps& m, const std::string& src, int count)
{
    offsets of;
    split(of, src, count);
    map(m, src, of);
}

namespace
{

void split(offsets& of, const std::string& src, int count)
{
    file_splitter fs(src);
    fs.split(of, count);
}

void mapf(std::size_t index, mapper& mr, const std::string& src, std::size_t begin, std::size_t end)
{
    std::ifstream f(src.c_str(), std::ifstream::binary);
    f.seekg(begin);
    std::string val;
    do
    {
       if (std::getline(f, val))
           mr.map(index, val);
    }
    while ((f) && (static_cast<decltype(end)>(f.tellg()) < end));
}

void map(maps& m, const std::string& src, offsets& of)
{
    mapper mr(of.size() - 1);
    std::list<std::thread> t;
    for (std::size_t i = 0; i < of.size() - 1; i++)
    {
        std::thread d(mapf, i, std::ref(mr), std::ref(src), of[i], of[i + 1] - 1);
        t.emplace_back(std::move(d));
    }
    for (auto& v : t)
        v.join();
    m = mr.result();
}

}
