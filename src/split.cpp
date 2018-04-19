#include <fstream>

#include "split.h"

namespace
{
std::size_t filesize(const std::string& path);
void do_split(offsets& offsets, std::ifstream& in, std::size_t offset, int count);
std::size_t do_pos(std::ifstream& in, char ch);
}

file_splitter::file_splitter(const std::string& path) : m_path(path)
{
}

void file_splitter::split(offsets& offsets, std::size_t count)
{
    std::size_t size = filesize(m_path);
    std::size_t offset = (size + count - size % count) / count;
    if (offset == 0)
        throw std::logic_error("offset and count do not matched");
    std::ifstream in(m_path.c_str(), std::ifstream::binary);
    if (!in)
        throw std::runtime_error("input file doesn't exist");
    do_split(offsets, in, offset, count);
}

namespace
{

std::size_t filesize(const std::string& path)
{
    std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

void do_split(offsets& o, std::ifstream& in, std::size_t offset, int count)
{
    o.push_back(0);
    in.seekg(offset, std::ios_base::cur);
    while ((in.eof() == false) && (o.size() != static_cast<std::size_t>(count)))
    {
        std::size_t off = do_pos(in, '\n');
        if (off != 0)
            o.push_back(off);
        else
            break;
        in.seekg(offset - 1, std::ios_base::cur);
    }
    if (in.eof() == false)
    {
        in.seekg(0, std::ios_base::end);
        o.push_back(in.tellg());
    }
}

std::size_t do_pos(std::ifstream& in, char ch)
{
    char c = 0;
    while (in.get(c) && (c != '\n'))
        /* nothing */;
    if (c == ch)
        return in.tellg();
    return 0;
}

}
