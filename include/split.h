#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "ymrt.h"

class file_splitter
{
    private:
        std::string m_path;
        std::size_t m_window;

    public:
        file_splitter(const std::string& path);
        void split(offsets& offsets, std::size_t count);
};
