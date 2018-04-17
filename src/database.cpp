#include <algorithm>

#include "dbexception.h"

#include "database.h"

void database::add_table(const std::string& name)
{
    auto tab = std::make_unique<table>(name);
    m_tables.emplace_back(std::move(tab));
}

table& database::find(const std::string& name)
{
    auto f = [ &name ](const table_type& t) { return t->get_name() == name; };
    auto i = std::find_if(m_tables.begin(), m_tables.end(), f);
    if (i == m_tables.end())
        throw table_not_found(name);
    return *(i->get());
}
