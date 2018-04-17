#include <iostream>

#include "dbexception.h"

#include "insert.h"

void insert::initialize(const operation_args& args)
{
    if (args.size() != 3)
        throw invalid_args_count("insert/0", 3);
    auto i = args.begin();
    m_table = *i;
    ++i;
    m_record.id = std::atoi((*i).c_str());
    ++i;
    m_record.name = *i;
}

table_name_list insert::affected() const
{
    table_name_list names = { m_table };
    return names;
}

void insert::apply(table_list& list)
{
    if (list.size() != 1)
        throw invalid_args_count("insert/1", 1);
    list[0]->insert(std::move(m_record));
}
