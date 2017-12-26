#include <cassert>
#include <cstdlib>
#include <iostream>

#include <algorithm>
#include <map>

#include "dummyc.h"
#include "map_alloc.h"

constexpr int fact(int val)
{
    return val > 1 ? val * fact(val - 1) : 1;
}

template <template <typename>class A, std::size_t... I>
auto gen_map(std::index_sequence<I...>)
{
    typedef int K;
    typedef int V;
    typedef std::map<K, V>::key_compare C;
    typedef std::map<K, V>::value_type VT;
#ifdef INIT_LIST
    return std::map<K, V, C, A<VT> > { std::make_pair(I, fact(I))... };
#else
    std::map<K, V, C, A<VT> > map;
    for (std::size_t s = 0; s < sizeof...(I); ++s)
        map.insert(std::make_pair(s, fact(s)));
#endif
    return map;
}

template <template <typename>class A, std::size_t... I>
auto gen_list(std::index_sequence<I...>)
{
    typedef int V;
#ifdef INIT_LIST
    return dummy_container<V, A<V> > { I... };
#else
    dummy_container<V, A<V> > list;
    for (std::size_t s = 0; s < sizeof...(I); ++s)
        list.insert(s);
#endif
    return list;
}

// #define TEST_EXT

int main(int, char**)
{
    constexpr std::size_t e_count = 10;
    try
    {
        gen_map<std::allocator>(std::make_index_sequence<e_count>{});
        auto map1 = gen_map<custom_allocator<e_count>::allocator>(std::make_index_sequence<e_count>{});
        auto f0 = [](decltype(map1)::const_reference v) { std::cout << v.first << " " << v.second << std::endl; };
        std::for_each(map1.begin(), map1.end(), f0);

        gen_list<std::allocator>(std::make_index_sequence<e_count>{});
        auto c1 = gen_list<custom_allocator<e_count>::allocator>(std::make_index_sequence<e_count>{});
        auto f1 = [](decltype(c1)::const_reference v) { std::cout << v <<std::endl; };
        std::for_each(c1.cbegin(), c1.cend(), f1);
#ifdef TEST_EXT
        for (int i = 10; i < 50; i++)
            c1.insert(i);
        std::for_each(c1.cbegin(), c1.cend(), f1);
#endif
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
