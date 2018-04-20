#include <algorithm>
#include <functional>

#include "shuffler.h"

shuffler::shuffler(int r) : m_m(r), m_locks(r)
{
}

void shuffler::emplace(std::string& str)
{
#if 0
    std::hash<std::string> fn;
    std::size_t h = fn(str);
    std::size_t i = h % m_locks.size();
#else
    std::size_t i = str[0] % m_locks.size();
#endif
    std::unique_lock<std::mutex> g(m_locks[i]);
    m_m[i].emplace_back(std::move(str));
}

maps&& shuffler::result()
{
    return std::move(m_m);
}

void shuffler::sort(std::size_t i)
{
    m_m[i].sort();
}
