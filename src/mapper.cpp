#include <algorithm>

#include "mapper.h"

mapper::mapper(int m) : m_maps(m)
{
}

void mapper::map(std::size_t i, const std::string& str)
{
    auto& l = m_maps[i];
    auto p = std::find_if(l.begin(), l.end(), [ &str ](decltype(str) v) { return v >= str; });
    if (p == l.end())
        l.push_back(str);
    else if (p == l.begin())
        l.insert(l.begin(), str);
    else
        l.insert(p, str);
}

maps&& mapper::result()
{
    return std::move(m_maps);
}
