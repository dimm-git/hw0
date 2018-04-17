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

    operation_result res{};
    op_insert ins{};
    std::list<std::string> l = { "A", "0", "00" };
    ins.initialize(l);
    table_list lst = { &t };
    ins.apply(res, lst);
    size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 1);

    std::list<std::string> l1 = { "A", "1", "11"};
    ins.initialize(l1);
    ins.apply(res, lst);
    size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 2);

    BOOST_CHECK_THROW(ins.apply(res, lst), duplicate_pk);
}

BOOST_AUTO_TEST_CASE(test_truncate)
{
    database db;
    db.add_table("A");
    auto& t = db.find("A");
    std::size_t size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 0);


    operation_result res{};
    op_insert ins{};
    std::list<std::string> l = { "A", "0", "00" };
    std::list<std::string> l1 = { "A", "1", "11"};
    ins.initialize(l);
    table_list lst = { &t };
    ins.apply(res, lst);
    ins.initialize(l1);
    ins.apply(res, lst);
    size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 2);

    std::list<std::string> l2 = { "A" };
    op_truncate tr{};
    tr.initialize(l2);
    tr.apply(res, lst);
    size = std::distance(t.begin(), t.end());
    BOOST_CHECK(size == 0);
}

void fill(table& a, table& b)
{
    std::list<record> la = { { 0, "lean" }, { 1, "sweater" }, { 2, "frank" }, { 3, "violation" }, { 4,  "quality" }, { 5, "precision" } };
    std::list<record> lb = { { 3, "proposal" }, { 4,  "example" }, { 5, "lake" }, { 6, "flour" }, { 7, "wonder" }, { 8, "selection" } };
    for (auto v : la)
        a.insert(std::move(v));
    for (auto v : lb)
        b.insert(std::move(v));
    std::size_t size = std::distance(a.begin(), a.end());
    BOOST_CHECK(size == la.size());
    size = std::distance(b.begin(), b.end());
    BOOST_CHECK(size == lb.size());
}

BOOST_AUTO_TEST_CASE(test_intersect)
{
    database db;
    db.add_table("A");
    db.add_table("B");
    table& a = db.find("A");
    table& b = db.find("B");
    fill(a, b);
    operation_result res{};
    op_intersect is{};
    std::list<std::string> l = { "A", "B" };
    is.initialize(l);
    table_list lst = { &a, &b };
    is.apply(res, lst);
    std::size_t size = std::distance(res.begin(), res.end());
    BOOST_CHECK(size == 3);
}

BOOST_AUTO_TEST_CASE(test_sym_diff)
{
    database db;
    db.add_table("A");
    db.add_table("B");
    table& a = db.find("A");
    table& b = db.find("B");
    fill(a, b);
    operation_result res{};
    op_sym_diff sd{};
    std::list<std::string> l = { "A", "B" };
    sd.initialize(l);
    table_list lst = { &a, &b };
    sd.apply(res, lst);
    std::size_t size = std::distance(res.begin(), res.end());
    BOOST_CHECK(size == 6);
}

BOOST_AUTO_TEST_SUITE_END()
