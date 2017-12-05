#define BOOST_TEST_MODULE test_version_def
#include <boost/test/unit_test.hpp>
#include <sstream>
#include "include/otus0.h"

BOOST_AUTO_TEST_SUITE(test_suite)

BOOST_AUTO_TEST_CASE(test_version)
{
    std::stringstream ss;
    ss << _VERSION_MAJOR << "." << _VERSION_MINOR << "." << _VERSION_PATCH;
    std::string s = ss.str();
    BOOST_CHECK(s == _VERSION_FULL_STRING);
}

BOOST_AUTO_TEST_SUITE_END()
