#pragma once

#include <ostream>
#include <type_traits>

#include <typeinfo>
#include <tuple>

namespace impl
{

template <typename T, std::size_t N>
struct check_tuple_fields
{
    static constexpr bool check()
    {
        typedef typename std::tuple_element<N - 1, T>::type U;
        typedef typename std::tuple_element<N - 2, T>::type V;

        if (std::is_same<U, V>::value == false)
            return false;
        return check_tuple_fields<T, N - 1>::check();
    }
};

template <typename T>
struct check_tuple_fields<T, 1>
{
    static constexpr bool check()
    {
        return true;
    }
};

template <typename T>
struct check_tuple
{
    static constexpr bool value = check_tuple_fields<T, std::tuple_size<T>::value>::check();
};

template <typename T, std::size_t N>
struct print_tuple
{
    static void print(std::ostream& s, const T& t)
    {
        print_tuple<T, N - 1>::print(s, t);
        s << "." << std::get<N - 1>(t);
    }
};

template <typename T>
struct print_tuple<T, 1>
{
    static void print(std::ostream& s, const T& t)
    {
        s << std::get<0>(t);
    }
};

}
