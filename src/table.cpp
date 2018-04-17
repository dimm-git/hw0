#include "dbexception.h"

#include "table.h"

bool record_less::operator()(const record& l, const record& r) const
{
    return l.id < r.id;
}

table::table(std::string name) : m_name(name)
{
}

void table::insert(record&& rec)
{
    auto ret = m_records.insert(rec);
    if (ret.second == false)
        throw duplicate_pk(rec.name, rec.id);
}

void table::truncate()
{
    m_records.clear();
}

const std::string table::get_name() const
{
    return m_name;
}

table::iterator table::begin()
{
    return m_records.begin();
}

table::iterator table::end()
{
    return m_records.end();
}

table::const_iterator table::cbegin() const
{
    return m_records.cbegin();
}

table::const_iterator table::cend() const
{
    return m_records.cend();
}
