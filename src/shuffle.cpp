#include <string>
#include <list>
#include <thread>

#include "shuffler.h"

namespace
{
void shufflef(std::list<std::string>& l, shuffler& sh);
}

void make_shuffle(maps& s, maps& r, int cr)
{
    shuffler sh(cr);
    std::list<std::thread> t;
    for (std::size_t i = 0; i < r.size(); i++)
    {
        std::thread d(shufflef, std::ref(r[i]), std::ref(sh));
        t.emplace_back(std::move(d));
    }
    for (auto& v : t)
        v.join();
    s = sh.result();
}

namespace
{
void shufflef(std::list<std::string>& l, shuffler& sh)
{
    for (auto& v : l)
        sh.emplace(v);
}
}
