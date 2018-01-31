#pragma once

#include <tuple>

#include "composite.h"
#include "index_proxy.h"
#include "matrix_iterator.h"

template <typename T, T DefVal, std::size_t Dim = 2>
class Matrix
{
    private:
        subdim<T, DefVal> root;
        index_proxy<T, DefVal, 0> pr;

    public:
        Matrix() : root(nullptr, -1), pr(&root) { }
        Matrix(const Matrix&) = delete;
        Matrix(Matrix&&) = delete;
        Matrix& operator=(const Matrix&) = delete;
        Matrix& operator=(Matrix&&) = delete;


        index_proxy<T, DefVal, Dim> operator[](std::size_t i)
        {
            return index_proxy<T, DefVal, Dim>(pr, i, root.at(i));
        }

        std::size_t size() const
        {
            return root.size();
        }

        matrix_iterator<T, Dim> begin()
        {
            return matrix_iterator<T, Dim>(&root);
        }

        matrix_iterator<T, Dim> end()
        {
            return matrix_iterator<T, Dim>();
        }
};

template <typename T, T DefVal>
class Matrix<T, DefVal, 1>
{
};
