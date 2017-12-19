#pragma once

#include <string>
#include <vector>

#include "ip_tools.h"

auto split(const std::string &str, char d)
{
    std::vector<std::string> r;

    decltype(str.find_first_of(d)) start = 0;
    auto stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.emplace_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(str.substr(start));

    return r;
}

auto read_pool(std::istream& s)
{
    std::vector<ip_parts> ip_pool;
    for (std::string line; std::getline(s, line);)
    {
        auto v = std::move(split(line, '\t'));
        auto parts = std::move(split(v.at(0), '.'));
        ip_pool.emplace_back(vector_to_ip_tuple(parts));
    }
    return ip_pool;
}
