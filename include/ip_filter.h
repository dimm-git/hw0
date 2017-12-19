#pragma once

#include <utility>

namespace aux
{
    template <std::size_t... I, typename T, class... Args>
    bool filter_tuple_impl(std::index_sequence<I...>, const T& t, std::size_t N, Args... args)
    {
        typedef typename std::tuple_element<0, T>::type TV;
        TV parts[] = { std::get<I>(t)... };
        TV vals[] = { args... };
        std::size_t i = 0;
        bool res = true;
        std::for_each(parts, parts + N, [ & ](const TV& val) { if (vals[i] != val) res = false; i++; } );
        return res;
    }

    template <typename T, class... Args>
    bool filter_tuple(const T& t, Args... args)
    {
        constexpr auto M = std::tuple_size<T>::value;
        constexpr auto N = std::min(sizeof...(args), M);

        if (M < sizeof...(args))
            return false;

        auto i = std::make_index_sequence<N>{};

        return filter_tuple_impl(i, t, N, args...);
    }

    template <typename T, typename V, std::size_t N>
    struct filter_tuple_any
    {
        static bool filter(const T& t, const V& v)
        {
            if (std::get<N - 1>(t) == v)
                return true;
            return filter_tuple_any<T, V, N - 1>::filter(t, v);
        }
    };

    template <typename T, typename V>
    struct filter_tuple_any<T, V, 1>
    {
        static bool filter(const T& t, const V& v)
        {
            return std::get<0>(t) == v;
        }
    };
}

template <typename T, class... Values>
auto filter(const T& pool, Values... vals)
{
    T ret;
    std::copy_if(pool.cbegin(), pool.cend(), std::back_inserter(ret), [ & ](decltype(*pool.cbegin())& v) { return aux::filter_tuple(v, vals...); });
    return ret;
}

template <typename T>
auto filter_any(const T& pool, int val)
{
    T ret;
    auto filter = [ & ](decltype(*pool.cbegin())& v)
    {
        typedef typename std::remove_reference<decltype(v)>::type Z;
        constexpr std::size_t N = std::tuple_size<Z>::value;
        return aux::filter_tuple_any<Z, decltype(val), N>::filter(v, val);
    };
    std::copy_if(pool.cbegin(), pool.cend(), std::back_inserter(ret), filter);
    return ret;
}
