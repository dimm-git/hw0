#pragma once

#include <stdint.h>
#include <ostream>
#include <type_traits>

namespace impl
{

template <typename T, std::size_t SZ>
struct print_ord
{
    static void print(std::ostream& strm, const T& t)
    {
        auto* ptr = reinterpret_cast<const uint8_t*>(&t);
        strm << +ptr[sizeof(t) - 1];
        for (std::size_t i = sizeof(t) - 1; i > 0; i--)
            strm << "." << +ptr[i - 1];
    }
};

}
