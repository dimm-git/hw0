#include <fstream>

#include "split.h"

splitter::splitter(const std::string& path, std::size_t window_size) : m_path(path), m_window(window_size)
{
}

void splitter::split(offsets& offsets, std::size_t count)
{
}
