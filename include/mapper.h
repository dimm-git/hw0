#pragma once

#include <list>
#include <vector>
#include <string>

#include "ymrt.h"

#include "split.h"

class mapper
{
    private:
        maps m_maps;

    public:
        mapper(int m);
        void map(std::size_t i, const std::string& str);
        maps&& result();
};
