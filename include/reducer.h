#pragma once

#include "ymrt.h"

class reducer
{
    private:
        results m_lst;
        results::iterator m_start = m_lst.end();

    public:
        reducer();
        void reduce(const std::string& str);
        results&& result();

    private:
        void add_item(const std::string& str);
        void update_item(const std::string& str);
        void build_prefix(prefix& l, prefix& r);
};
