#pragma once

#include <list>
#include <memory>
#include <string>

#include "table.h"

class database
{
    private:
        using tables = std::list<table>;
        tables m_tables;

    public:
        void add_table(const std::string& name);
        table& find(const std::string& name);
};
