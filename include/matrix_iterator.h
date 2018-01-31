#pragma once

#include <vector>

#include "composite.h"

namespace impl
{

template <typename T, std::size_t... I>
auto a2t_impl(const std::vector<composite<T>*>& a, std::vector<std::size_t>& offset, const T& val, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]->off(offset[I + 1])->index()..., val);
}

template <std::size_t N, typename T>
auto a2t(std::vector<composite<T>*>& a, std::vector<std::size_t>& offset, const T& val)
{
    return a2t_impl(a, offset, val, std::make_index_sequence<N> {});
}

}

template <typename T, std::size_t Dim>
class matrix_iterator
{
    private:
        std::vector<composite<T>*> dims;
        std::vector<std::size_t> offset;
        composite<T>* root = nullptr;

        void initialize()
        {
            if (root->length() > 0)
            {
                dims[0] = root;
                for (std::size_t i = 1; i < offset.size(); i++)
                    dims[i] = dims[i - 1]->off(offset[i]);
            }
            else
            {
                offset[0] = 1;
            }
        }

        bool advance(std::size_t i = Dim)
        {
            offset[i] = offset[i] + 1;

            if (i == 0)
                return false;

            composite<T>* val = dims[i - 1]->off(offset[i]);

            if (val != nullptr)
            {
                dims[i] = val;
                return true;
            }

            offset[i] = 0;
            if (advance(i - 1) == true)
            {
                dims[i] = dims[i - 1]->off(0);
                return true;
            }
            return false;
        }

    public:
        typedef T value_type;
        typedef std::forward_iterator_tag iterator_category;

        matrix_iterator(composite<T>* rt) : dims(Dim + 1, nullptr), offset(Dim + 1, 0), root(rt)
        {
            initialize();
        }

        matrix_iterator() : offset(Dim + 1, 0)
        {
            offset[0] = 1;
        }

        matrix_iterator operator++(int)
        {
            matrix_iterator<T, Dim> tmp = *this;
            if (root != nullptr)
                advance();
            return tmp;
        }

        matrix_iterator& operator++()
        {
            if (root != nullptr)
                advance();
            return *this;
        }

        auto operator*()
        {
            const T& val = dims[Dim - 1]->off(offset[Dim])->value();
            return impl::a2t<Dim>(dims, offset, val);
        }

        bool operator==(const matrix_iterator<T, Dim>& o)
        {
            return offset == o.offset;
        }

        bool operator!=(const matrix_iterator<T, Dim>& o)
        {
            return offset != o.offset;
        }
};
