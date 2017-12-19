#pragma once

#include <algorithm>
#include <ostream>
#include <vector>
#include <tuple>

// sorry
typedef std::tuple<int, int, int, int> ip_parts;

namespace aux
{
    template <std::size_t... I, typename T>
    auto vector_to_ip_tuple_impl(std::index_sequence<I...>, const std::vector<T>& v)
    {
        auto f = [ & ](std::size_t i) { return (i < v.size()) ? std::stoi(v[i]) : 0; };
        return std::make_tuple(f(I)...);
    }

    template <typename T, std::size_t N>
    struct tuple_printer
    {
        static void print(const T& t, std::ostream& s)
        {
            tuple_printer<T, N - 1>::print(t, s);
            s << "." << std::get<N - 1>(t);
        }
    };
 
    template <typename T>
    struct tuple_printer<T, 1>
    {
        static void print(const T& t, std::ostream& s) 
        {
            s << std::get<0>(t);
        }
    };
}

template <typename T>
auto vector_to_ip_tuple(const std::vector<T>& v)
{
    constexpr auto N = std::tuple_size<ip_parts>::value;
    auto i = std::make_index_sequence<N>{};
    return aux::vector_to_ip_tuple_impl(i, v);
}

template <typename T>
void print_pool(const T& pool, std::ostream& stream)
{
    typedef typename std::remove_reference<decltype(pool[0])>::type N;
    constexpr auto M = std::tuple_size<N>::value;
    auto printer = [ & ](const N& v)
    {
        aux::tuple_printer<N, M>::print(v, stream);
        stream << std::endl;
    };
    std::for_each(pool.cbegin(), pool.cend(), printer);
}

template <typename T>
void ip_sort(T& t)
{
    auto less = [](decltype(*t.cbegin()) l, decltype(*t.cbegin()) r) -> bool
    {
        return l > r;
    };
    std::sort(t.begin(), t.end(), less);
}
