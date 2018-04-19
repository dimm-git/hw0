#pragma once

#include <mutex>
#include <vector>

#include "ymrt.h"

class shuffler
{
    private:
        maps m_m;
        std::vector<std::mutex> m_locks;

    public:
        shuffler(int r);
        void emplace(std::string& str);
        maps&& result();
};
