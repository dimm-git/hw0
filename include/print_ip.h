#pragma once
/** @file */

#include "print_container.h"
#include "print_ord.h"
#include "print_tuple.h"


template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type print_ip(std::ostream& strm, const T& val)
{
    impl::print_ord<T>::print(strm, val);
}

template <class... Args>
// typename std::enable_if<impl::check_tuple<T>::value, void>::type print_ip(std::ostream& strm, const T& val)
auto print_ip(std::ostream& strm, const std::tuple<Args...>& val) -> typename std::enable_if<impl::check_tuple<std::remove_reference_t<decltype(val)> >::value, void>::type
{
    typedef typename std::remove_reference<decltype(val)>::type U;
    impl::print_tuple<U, std::tuple_size<U>::value>::print(strm, val);
}

template <typename T>
typename std::enable_if<impl::is_container<T>::value, void>::type print_ip(std::ostream& strm, const T& val)
{
    impl::print_container<T>::print(strm, val);
}
