#pragma once

#include <algorithm>
#include <ostream>
#include <type_traits>

namespace impl
{

template <typename...>
using void_t = void;

template<typename T, typename = void>
struct has_begin : std::false_type
{
};

template<typename T>
struct has_begin<T, void_t<decltype(&T::cbegin)> > : std::is_member_function_pointer<decltype(&T::cbegin)>
{
};

template<typename T, typename = void>
struct has_end : std::false_type
{
};

template<typename T>
struct has_end<T, void_t<decltype(&T::cend)> > : std::is_member_function_pointer<decltype(&T::cend)>
{
};


template <typename T>
struct is_container
{
    static constexpr bool value = has_begin<T>::value && has_end<T>::value;
};

template <typename T>
struct print_container
{
    static void print(std::ostream& s, const T& t)
    {
        auto i = t.begin();
        s << *i;
        ++i;
        std::for_each(i, t.end(), [ &s ](const decltype(*i)& v) { s << "." << v; } );
    }
};

}
