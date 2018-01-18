#pragma once

#include <stdint.h>
#include <ostream>
#include <type_traits>

namespace impl
{

template <typename T, std::size_t SZ = sizeof(T), bool S = std::is_signed<T>::value>
struct print_ord
{
    static void print(std::ostream& s, const T& v)
    {
        s << v;
    }
};

template <typename T, std::size_t SZ>
struct print_ord<T, SZ, false>
{
    static void print(std::ostream& strm, const T& t)
    {
        auto* ptr = reinterpret_cast<const uint8_t*>(&t);
        strm << +ptr[sizeof(t) - 1];
        for (std::size_t i = sizeof(t) - 1; i > 0; i--)
            strm << "." << +ptr[i - 1];
    }
};

template <typename T, std::size_t SZ>
struct print_ord<T, SZ, true>
{
    static void print(std::ostream& strm, const T& val)
    {
        auto x = static_cast<std::make_unsigned_t<T> >(val);
        print_ord<decltype(x), sizeof(x), false>::print(strm, x);
    }
};

template<>
struct print_ord<uint8_t, 1, true>
{
    static void print(std::ostream& strm, const unsigned char& val)
    {
        strm << +val;
    }
};

template<>
struct print_ord<bool, 1, false>
{
    static void print(std::ostream& strm, const bool& val)
    {
        auto v = static_cast<uint8_t>(val);
        print_ord<decltype(v)>::print(strm, v);
    }
};

}
