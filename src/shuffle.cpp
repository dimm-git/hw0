#include <string>
#include <list>
#include <thread>

#include "shuffler.h"

namespace
{
void shufflef(std::list<std::string>& l, shuffler& sh);

void emplace(shuffler& sh, maps& r);
void sort(shuffler& sh, std::size_t count);
}

void make_shuffle(maps& s, maps& r, int cr)
{
    shuffler sh(cr);
    emplace(sh, r);
    sort(sh, cr);
    s = sh.result();
}

namespace
{
void shufflef(std::list<std::string>& l, shuffler& sh)
{
    for (auto& v : l)
        sh.emplace(v);
}

void emplace(shuffler& sh, maps& r)
{
    std::list<std::thread> t;
    for (std::size_t i = 0; i < r.size(); i++)
    {
        std::thread d(shufflef, std::ref(r[i]), std::ref(sh));
        t.emplace_back(std::move(d));
    }
    for (auto& v : t)
        v.join();

}

void sortf(shuffler& sh, std::size_t i)
{
    sh.sort(i);
}

void sort(shuffler& sh, std::size_t count)
{
    std::list<std::thread> t;
    for (std::size_t i = 0; i < count; i++)
    {
        std::thread d(sortf, std::ref(sh), i);
        t.emplace_back(std::move(d));
    }
    for (auto& v : t)
        v.join();
}

}
