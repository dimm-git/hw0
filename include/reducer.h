#pragma once

#include "ymrt.h"

class reducer
{
    private:
        std::list<std::string> m_lst;

    public:
        reducer();
        void reduce(const std::string& str);
        std::list<std::string>&& result();
};
