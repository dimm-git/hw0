#pragma once

#include <string>
#include <vector>

class splitter
{
    private:
        std::string m_path;
        std::size_t m_window;

    public:
        using offsets = std::vector<std::size_t>;

    public:
        splitter(const std::string& path, std::size_t window_size);
        void split(offsets& offsets, std::size_t count);
};
