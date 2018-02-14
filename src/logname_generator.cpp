#include <sys/time.h>

#include <sstream>

#include "logname_generator.h"

void logname_generator_prod::update_timestamp()
{
    if (m_ts == 0)
        m_ts = std::time(nullptr);
}

std::string logname_generator_prod::name()
{
    std::stringstream ss;
    ss << "bulk" << m_ts << ".log";
    m_ts = 0;
    return ss.str();
}

static bool operator==(const timeval& l, const timeval& r)
{
    return (l.tv_sec == r.tv_sec) && (l.tv_usec == r.tv_usec);
}

void logname_generator_debug::update_timestamp()
{
    if (m_tv == timeval { 0, 0 })
        gettimeofday(&m_tv, nullptr);
}

std::string logname_generator_debug::name()
{
    std::stringstream ss;
    ss << "bulk" << m_tv.tv_sec << "_" << m_tv.tv_usec << ".log";
    m_tv = timeval { 0, 0 };
    return ss.str();
}
