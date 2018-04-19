#include "reducer.h"

reducer::reducer()
{
}

void reducer::reduce(const std::string& str)
{
}

std::list<std::string>&& reducer::result()
{
    return std::move(m_lst);
}
