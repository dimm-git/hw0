#include <iostream>
#include <sstream>
#include <stdexcept>

#include "app_mode.h"

std::istream& app_mode_prod::get_istream()
{
    return std::cin;
}

int app_mode_prod::get_block_size()
{
    return m_depth;
}

void app_mode_prod::parse(int argc, const char* argv[])
{
    if (argc != 2)
        throw std::runtime_error(usage(argv[0]));

    std::stringstream ss;
    ss << argv[1];
    ss >> m_depth;
}

std::string app_mode_prod::usage(const char* app)
{
    std::stringstream ss;
    ss << "Usage: " << app << " <block size>";
    return ss.str();
}

logname_generator& app_mode_prod::get_generator()
{
    return m_gen;
}

std::istream& app_mode_debug::get_istream()
{
    return m_strm;
}

int app_mode_debug::get_block_size()
{
    return m_depth;
}

std::size_t app_mode_prod::get_queue_length() const noexcept
{
    return 1;
}

void app_mode_debug::parse(int argc, const char* argv[])
{
    if (argc < 3)
        throw std::runtime_error(usage(argv[0]));

    std::stringstream ss;
    ss << argv[1];
    ss >> m_depth;
    m_strm.open(argv[2], std::ifstream::in);

    if (argc >= 4)
    {
        ss.str("");
        ss << argv[3];
        ss >> m_ql;
    }
}

std::string app_mode_debug::usage(const char* app)
{
    std::stringstream ss;
    ss << "Usage: " << app << " <block size> <input file name> [<queue length>]";
    return ss.str();
}

logname_generator& app_mode_debug::get_generator()
{
    return m_gen;
}

std::size_t app_mode_debug::get_queue_length() const noexcept
{
    return m_ql;
}
