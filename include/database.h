#pragma once

#include <list>
#include <memory>
#include <string>

#include "table.h"

using table_type = std::unique_ptr<table>;

class database
{
    private:
        using tables = std::list<table_type>;
        tables m_tables;

    public:
        void add_table(const std::string& name);
        table& find(const std::string& name);
};
