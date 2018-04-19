#include <algorithm>
#include <functional>

#include "shuffler.h"

shuffler::shuffler(int r) : m_m(r), m_locks(r)
{
}

void shuffler::emplace(std::string& str)
{
    std::hash<std::string> fn;
    std::size_t h = fn(str);
    std::size_t i = h % m_locks.size();
    std::unique_lock<std::mutex> g(m_locks[i]);

    auto& l = m_m[i];
    auto p = std::find_if(l.begin(), l.end(), [ &str ](decltype(str) v) { return v >= str; });
    if (p == l.end())
        l.emplace_back(str);
    else if (p == l.begin())
        l.insert(l.begin(), std::move(str));
    else
        l.insert(p, std::move(str));

}

maps&& shuffler::result()
{
    return std::move(m_m);
}
