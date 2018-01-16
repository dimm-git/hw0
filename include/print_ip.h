#pragma once
/*! \mainpage
   ::print_ip - семейство шаблонных функций записи целых чисел, полей кортежей и контейнеров в поток
*/

#include "print_container.h"
#include "print_ord.h"
#include "print_tuple.h"


//! @brief Функция вывода в поток целых чисел
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type print_ip(std::ostream& strm, const T& val)
{
    impl::print_ord<T>::print(strm, val);
}

//! @brief Функция вывода в поток полей кортежей.
//! @warning Поля кортежа должны быть одного типа
//! @warning В отличии от версии для целых чисел, нет специализаии для char и signed типов
template <class... Args>
// typename std::enable_if<impl::check_tuple<T>::value, void>::type print_ip(std::ostream& strm, const T& val)
auto print_ip(std::ostream& strm, const std::tuple<Args...>& val) -> typename std::enable_if<impl::check_tuple<std::remove_reference_t<decltype(val)> >::value, void>::type
{
    typedef typename std::remove_reference<decltype(val)>::type U;
    impl::print_tuple<U, std::tuple_size<U>::value>::print(strm, val);
}

//! @brief Функция вывода в поток контейнера (класса с константным итератором).
//! @warning В отличии от версии для целых чисел, нет специализаии для char и signed типов
template <typename T>
typename std::enable_if<impl::is_container<T>::value, void>::type print_ip(std::ostream& strm, const T& val)
{
    impl::print_container<T>::print(strm, val);
}
