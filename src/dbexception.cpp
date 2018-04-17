#include <sstream>

#include "dbexception.h"

duplicate_pk::duplicate_pk(const std::string&, int id)
{
    std::stringstream s;
    s << "duplicate " << id;
    m_text = s.str();
}

const char* duplicate_pk::what() const throw()
{
    return m_text.c_str();
}

table_not_found::table_not_found(const std::string& tab_name)
{
    std::stringstream s;
    s << "table " << tab_name << " not found";
    m_text = s.str();
}

const char* table_not_found::what() const throw()
{
    return m_text.c_str();
}

invalid_args_count::invalid_args_count(const std::string& name, int count)
{
    std::stringstream s;
    s << "invalid operation " << name << " args count. expected " << count;
    m_text = s.str();
}

const char* invalid_args_count::what() const throw()
{
    return m_text.c_str();
}
