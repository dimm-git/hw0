#define BOOST_TEST_MODULE test_modules

#include <boost/test/unit_test.hpp>
#include <iostream>

#include "matrix.h"


BOOST_AUTO_TEST_SUITE(test_suite)

BOOST_AUTO_TEST_CASE(test_2d)
{
    Matrix<int, -1, 2> matrix;
    BOOST_CHECK(matrix.size() == 0);
    auto a = matrix[0][0];
    BOOST_CHECK(a == -1);
    BOOST_CHECK(matrix.size() == 0);

    matrix[0][0] = -1;
    BOOST_CHECK(matrix.size() == 0);

    matrix[100][100] = 314;
    BOOST_CHECK(matrix.size() == 1);
    BOOST_CHECK(matrix[100][100] == 314);
    int count = 0;
    for (auto c : matrix)
    {
        std::size_t x = 0;
        std::size_t y = 0;
        int v = 0;
        std::tie(x, y, v) = c;
        BOOST_CHECK(x == 100 && y == 100 && v == 314);
        ++count;
    }
    BOOST_CHECK(count == 1);
    matrix[100][100] = -1;
    BOOST_CHECK(matrix.size() == 0);
    BOOST_CHECK(matrix.begin() == matrix.end());
    std::vector<std::tuple<std::size_t, std::size_t, int> > tv = {
        std::make_tuple(0, 0, 0),
        std::make_tuple(0, 1, 1),
        std::make_tuple(1, 0, 10),
        std::make_tuple(100, 100, 100100)
    };
    for (auto t: tv)
        matrix[std::get<0>(t)][std::get<1>(t)] = std::get<2>(t);
    for (auto c : matrix)
    {
        auto i = std::find(tv.begin(), tv.end(), c);
        BOOST_CHECK(i != tv.end());
        BOOST_CHECK(matrix[std::get<0>(*i)][std::get<1>(*i)] == std::get<2>(*i));
        tv.erase(std::remove(tv.begin(), tv.end(), c), tv.end());
    }
    BOOST_CHECK(tv.size() == 0);
    std::size_t sz = matrix.size();
    matrix[0][0] = -1;
    BOOST_CHECK(sz == matrix.size() + 1);
}

BOOST_AUTO_TEST_CASE(test_3d)
{
    Matrix<int, -1, 3> matrix;
    BOOST_CHECK(matrix.size() == 0);
    auto a = matrix[0][0][0];
    BOOST_CHECK(a == -1);
    BOOST_CHECK(matrix.size() == 0);
    matrix[100][100][100] = 314;
    BOOST_CHECK(matrix[100][100][100] == 314);
    BOOST_CHECK(matrix.size() == 1);
    int count = 0;
    for (auto c : matrix)
    {
        std::size_t x = 0;
        std::size_t y = 0;
        std::size_t z = 0;
        int v = 0;
        std::tie(x, y, z, v) = c;
        BOOST_CHECK(x == 100 && y == 100 && z == 100 && v == 314);
        ++count;
    }
    BOOST_CHECK(count == 1);
    matrix[100][100][100] = -1;
    BOOST_CHECK(matrix.size() == 0);
    BOOST_CHECK(matrix.begin() == matrix.end());

    std::vector<std::tuple<std::size_t, std::size_t, std::size_t, int> > tv = {
        std::make_tuple(0, 0, 0, 0),
        std::make_tuple(0, 1, 1, 0),
        std::make_tuple(1, 0, 0, 100),
        std::make_tuple(100, 100, 100, 100100100)
    };
    for (auto t: tv)
        matrix[std::get<0>(t)][std::get<1>(t)][std::get<2>(t)] = std::get<3>(t);
    for (auto c : matrix)
    {
        auto i = std::find(tv.begin(), tv.end(), c);
        BOOST_CHECK(i != tv.end());
        BOOST_CHECK(matrix[std::get<0>(*i)][std::get<1>(*i)][std::get<2>(*i)] == std::get<3>(*i));
        tv.erase(std::remove(tv.begin(), tv.end(), c), tv.end());
    }
    BOOST_CHECK(tv.size() == 0);
    std::size_t sz = matrix.size();
    matrix[0][0][0] = -1;
    BOOST_CHECK(sz == matrix.size() + 1);
}

BOOST_AUTO_TEST_SUITE_END()
