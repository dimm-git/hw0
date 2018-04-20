#include "reducer.h"

reducer::reducer()
{
}

void reducer::reduce(const std::string& str)
{
    if (m_lst.empty() == false)
        update_item(str);
    else
    {
        add_item(str);
        m_start = m_lst.begin();
    }
}

results&& reducer::result()
{
    return std::move(m_lst);
}

void reducer::add_item(const std::string& str)
{
    prefix r(str, "", 1);
    m_lst.emplace_back(r);
}

void reducer::update_item(const std::string& str)
{
    auto& r = m_lst.back();
    if (std::get<0>(r) == str)
        std::get<2>(r)++;
    else
    {
        add_item(str);
        build_prefix(r, m_lst.back());
    }
}

void reducer::build_prefix(prefix& l, prefix& r)
{
    const auto& ls = std::get<0>(l);
    const auto& rs = std::get<0>(r);
    // :FIXME: r can't be <= l
    auto diff = std::mismatch(ls.begin(), ls.end(), rs.begin());
    std::get<1>(r) = rs.substr(0, std::distance(rs.begin(), diff.second) + 1);
    if (std::get<1>(l).size() == 0)
        std::get<1>(l) = ls.substr(0, std::distance(ls.begin(), diff.first) + 1);
}
