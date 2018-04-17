#define BOOST_TEST_MODULE jointst

#include <boost/test/unit_test.hpp>

#include <iterator>
#include <list>

#include "database.h"
#include "dbexception.h"
#include "dbops.h"

BOOST_AUTO_TEST_SUITE(test_suite)

BOOST_AUTO_TEST_CASE(test_insert)
{
    database db;
    db.add_table("A");
    db.add_table("B");
    auto& t = db.find("A");
    std::size_t size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 0);

    insert ins{};
    std::list<std::string> l = { "A", "0", "00" };
    ins.initialize(l);
    table_list lst = { &t };
    ins.apply(lst);
    size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 1);

    std::list<std::string> l1 = { "A", "1", "11"};
    ins.initialize(l1);
    ins.apply(lst);
    size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 2);

    BOOST_CHECK_THROW(ins.apply(lst), duplicate_pk);
}

BOOST_AUTO_TEST_SUITE_END()
