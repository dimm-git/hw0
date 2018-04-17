#include <algorithm>
#include <iostream>

#include "dbexception.h"

#include "dbops.h"

class result_proxy
{
    private:
        operation_result& m_res;
        table& m_second;

    public:
        result_proxy(operation_result& res, table& second);
        result_proxy& operator=(const record_type& rec);
        result_proxy& operator=(const result_record& rec);
};

class isect_iterator
{
    private:
        operation_result& m_res;
        table& m_second;

    public:
        isect_iterator(operation_result& res, table& second);
        isect_iterator& operator++();
        isect_iterator operator++(int);
        result_proxy operator*();
};

void op_insert::initialize(const operation_args& args)
{
    if (args.size() != 3)
        throw invalid_args_count("insert/0", 3);
    auto i = args.begin();
    m_table = *i;
    ++i;
    m_record.id = std::atoi((*i).c_str());
    ++i;
    m_record.name = *i;
}

table_name_list op_insert::affected() const
{
    table_name_list names = { m_table };
    return names;
}

void op_insert::apply(operation_result&, table_list& list)
{
    if (list.size() != 1)
        throw invalid_args_count("insert/1", 1);
    list[0]->insert(std::move(m_record));
}

void op_truncate::initialize(const operation_args& args)
{
    if (args.size() != 1)
        throw invalid_args_count("truncate/0", 1);
    m_table = *args.begin();
}

table_name_list op_truncate::affected() const
{
    table_name_list names = { m_table };
    return names;
}

void op_truncate::apply(operation_result&, table_list& list)
{
    if (list.size() != 1)
        throw invalid_args_count("truncate/1", 1);
    list[0]->truncate();
}

void op_intersect::initialize(const operation_args& args)
{
    if (args.size() != 2)
        throw invalid_args_count("intersect/0", 2);
    m_tab0 = *args.begin();
    m_tab1 = *(++args.begin());
}

table_name_list op_intersect::affected() const
{
    table_name_list names = { m_tab0, m_tab1 };
    return names;
}

void op_intersect::apply(operation_result& res, table_list& list)
{
    if (list.size() != 2)
        throw invalid_args_count("intersect/1", 1);
    isect_iterator it(res, *list[1]);
    auto cmp = [ ](const record_type& l, const record_type& r)
    {
        return l->id < r->id;
    };
    std::set_intersection(list[0]->begin(), list[0]->end(), list[1]->begin(), list[1]->end(), it, cmp);
}

result_proxy::result_proxy(operation_result& res, table& second) : m_res(res), m_second(second)
{
}

result_proxy& result_proxy::operator=(const record_type& rec)
{
    auto f = [ &rec ](const record_type& r) { return r->id == rec->id; };
    auto i = std::find_if(m_second.begin(), m_second.end(), f);
    if (i == m_second.end())
        throw record_not_found(m_second.get_name(), rec->id);
    result_record res = { rec->id, &rec->name, &(*i)->name };
    m_res.emplace_back(std::move(res));
    return *this;
}

result_proxy& result_proxy::operator=(const result_record& rec)
{
    m_res.emplace_back(std::move(rec));
    return *this;
}

isect_iterator::isect_iterator(operation_result& res, table& second) : m_res(res), m_second(second)
{
}

isect_iterator& isect_iterator::operator++()
{
    return *this;
}

isect_iterator isect_iterator::operator++(int)
{
    isect_iterator copy = *this;
    ++*this;
    return copy;
}

result_proxy isect_iterator::operator*()
{
    return result_proxy(m_res, m_second);
}

void op_sym_diff::initialize(const operation_args& args)
{
    if (args.size() != 2)
        throw invalid_args_count("sym_diff/0", 2);
    m_tab0 = *args.begin();
    m_tab1 = *(++args.begin());
}

table_name_list op_sym_diff::affected() const
{
    table_name_list names = { m_tab0, m_tab1 };
    return names;
}

template<class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt do_set_symmetric_difference(InputIt1 first1, InputIt1 last1,
                                     InputIt2 first2, InputIt2 last2,
                                     OutputIt d_first, Compare comp)
{
    auto copy_1 = [ &d_first ](typename InputIt1::reference i)
    {
        result_record r { i->id, &i->name, nullptr };
        *d_first = r;
        ++d_first;
    };

    auto copy_2 = [ &d_first ](typename InputIt2::reference i)
    {
        result_record r { i->id, nullptr, &i->name };
        *d_first = r;
        ++d_first;
    };

    while (first1 != last1)
    {
        if (first2 == last2)
        {
            std::for_each(first1, last1, copy_1);
            return d_first;
        }
 
        if (comp(*first1, *first2))
        {
            copy_1(*first1++);
        }
        else
        {
            if (comp(*first2, *first1))
                copy_2(*first2);
            else
                ++first1;
            ++first2;
        }
    }
    std::for_each(first2, last2, copy_2);
    return d_first;
}

void op_sym_diff::apply(operation_result& res, table_list& list)
{
    if (list.size() != 2)
        throw invalid_args_count("sym_diff/1", 1);
    auto cmp = [ ](const record_type& l, const record_type& r)
    {
        return l->id < r->id;
    };
    isect_iterator it(res, *list[0]);
    do_set_symmetric_difference(list[0]->begin(), list[0]->end(), list[1]->begin(), list[1]->end(), it, cmp);
}
