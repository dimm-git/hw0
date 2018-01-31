#include <cassert>
#include <iostream>

#include "matrix.h"

template <typename T>
void fill(T& matrix, std::size_t from, std::size_t to)
{
    for (std::size_t i = from; i <= to; i++)
    {
        matrix[i][i] = i;
        matrix[i][to - i] = to - i;
    }
}

template <typename T>
void print(T& matrix, std::size_t from, std::size_t to)
{
    for (std::size_t i = from; i <= to; i++)
    {
        for (std::size_t j = from; j <= to; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
}

template <typename T>
void iterate(T& matrix)
{
    for (auto c : matrix)
    {
        std::size_t i = 0;
        std::size_t j = 0;
        int v = 0;
        std::tie(i, j, v) = c;
        std::cout << i << " " << j << " " << v << std::endl;
    }
}

int main(int, char**)
{
    Matrix<int, 0, 2> matrix;
    fill(matrix, 0, 9);
    print(matrix, 1, 8);
    std::cout << matrix.size() << std::endl;
    iterate(matrix);
    return 0;
}
