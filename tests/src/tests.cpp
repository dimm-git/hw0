#define BOOST_TEST_MODULE test_modules
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ip_read.h"
#include "ip_tools.h"
#include "ip_filter.h"

BOOST_AUTO_TEST_SUITE(test_suite)

BOOST_AUTO_TEST_CASE(test_split)
{
    std::vector<std::tuple<std::string, char, std::vector<std::string> > > test_data {
      std::make_tuple("", '.', std::vector<std::string>{ "" }),
      std::make_tuple("11", '.', std::vector<std::string>{ "11" }),
      std::make_tuple("..", '.', std::vector<std::string>{ "", "", "" }),
      std::make_tuple("11.", '.',  std::vector<std::string>{ "11", "" }),
      std::make_tuple(".11", '.',  std::vector<std::string>{ "", "11" }),
      std::make_tuple("11.22", '.', std::vector<std::string>{ "11", "22" })
    };

    for (auto& t: test_data)
      BOOST_CHECK(split(std::get<0>(t), std::get<1>(t)) == std::get<2>(t));
}

// P1
template <typename Apply>
static void do_test(const std::string& first, const std::string& last, Apply apply)
{
  BOOST_CHECK(boost::unit_test::framework::master_test_suite().argc > 1);
  const std::string& path = boost::unit_test::framework::master_test_suite().argv[1];
  std::ifstream s(path);
  BOOST_CHECK(s.is_open());
  auto rd = std::move(read_pool(s));
  BOOST_CHECK(rd.size() == 1000);
  apply(rd);
  std::stringstream ss;
  print_pool(rd, ss);
  std::string res = ss.str();
  BOOST_CHECK(first.size() <= res.size());
  BOOST_CHECK(res.compare(0, first.size(), first) == 0);
  if (last.size() > 0)
  {
    BOOST_CHECK(res.compare(res.size() - last.size(), last.size(), last) == 0);
    BOOST_CHECK(last.size() <= res.size());
  }
}

BOOST_AUTO_TEST_CASE(test_read)
{
  std::string first = "113.162.145.156\n157.39.22.224\n79.180.73.190\n";
  std::string last = "67.183.105.87\n91.64.189.196\n93.179.90.82\n";
  do_test(first, last, [](decltype(read_pool(std::cin))&) { });
}

BOOST_AUTO_TEST_CASE(test_sort)
{
  std::string first = "222.173.235.246\n222.130.177.64\n222.82.198.61\n";
  std::string last = "1.70.44.170\n1.29.168.152\n1.1.234.8\n";
  do_test(first, last, ip_sort<decltype(read_pool(std::cin))>);
}

BOOST_AUTO_TEST_CASE(test_filter_1)
{
  std::string first = "1.29.168.152\n1.231.69.33\n1.1.234.8\n1.87.203.225\n1.70.44.170\n";
  std::string last = "";
  do_test(first, last, [](decltype(read_pool(std::cin))& t) { t = filter(t, 1); });
}

BOOST_AUTO_TEST_CASE(test_filter_2)
{
  std::string first = "46.70.113.73\n46.70.147.26\n46.70.225.39\n46.70.29.76\n";
  std::string last = "";
  do_test(first, last, [](decltype(read_pool(std::cin))& t) { t = filter(t, 46, 70); });
}

BOOST_AUTO_TEST_CASE(test_filter_any)
{
    std::string first = "186.204.34.46\n"
                        "186.46.222.194\n"
                        "185.46.87.231\n"
                        "185.46.86.132\n"
                        "185.46.86.131\n"
                        "185.46.86.131\n"
                        "185.46.86.22\n"
                        "185.46.85.204\n"
                        "185.46.85.78\n"
                        "68.46.218.208\n"
                        "46.251.197.23\n"
                        "46.223.254.56\n"
                        "46.223.254.56\n"
                        "46.182.19.219\n"
                        "46.161.63.66\n"
                        "46.161.61.51\n"
                        "46.161.60.92\n"
                        "46.161.60.35\n"
                        "46.161.58.202\n"
                        "46.161.56.241\n"
                        "46.161.56.203\n"
                        "46.161.56.174\n"
                        "46.161.56.106\n"
                        "46.161.56.106\n"
                        "46.101.163.119\n"
                        "46.101.127.145\n"
                        "46.70.225.39\n"
                        "46.70.147.26\n"
                        "46.70.113.73\n"
                        "46.70.29.76\n"
                        "46.55.46.98\n"
                        "46.49.43.85\n"
                        "39.46.86.85\n"
                        "5.189.203.46\n";

    std::string last = "";
    do_test(first, last, [](decltype(read_pool(std::cin)) &t) { ip_sort<decltype(t)>(t); t = filter_any(t, 46); });
}

BOOST_AUTO_TEST_SUITE_END()
