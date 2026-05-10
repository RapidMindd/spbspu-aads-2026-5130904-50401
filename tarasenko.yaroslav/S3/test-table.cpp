#include <boost/test/unit_test.hpp>
#include <string>
#include "hash_table.hpp"

using namespace tarasenko;

using HTable = HashTable< int, int, std::hash< int >, std::equal_to< int > >;

BOOST_AUTO_TEST_CASE(default_constructor)
{
  HTable table;
  BOOST_TEST(table.getSize() == 0);
  BOOST_TEST(table.getCapacity() == 64);
}

BOOST_AUTO_TEST_CASE(add)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(table.getSize() == 1);
}

BOOST_AUTO_TEST_CASE(drop)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(!table.drop(2));
  BOOST_TEST(table.drop(1));
  BOOST_TEST(table.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(get)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(table.get(1) == 1);
  BOOST_TEST(table.getSize() == 1);
}

BOOST_AUTO_TEST_CASE(get_empty)
{
  HTable table;
  BOOST_CHECK_THROW(table.get(1), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(has)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(table.has(1));
  BOOST_TEST(!table.has(2));
}

BOOST_AUTO_TEST_CASE(rehash)
{
  HTable table;
  table.add(1, 1);
  table.rehash(128);
  BOOST_TEST(table.getCapacity() == 128);
  BOOST_TEST(table.getSize() == 1);
  BOOST_TEST(table.has(1));
}

BOOST_AUTO_TEST_CASE(default_template_parameters)
{
  HashTable< int, std::string > table;
  table.add(1, "hello");
  BOOST_TEST(table.get(1) == "hello");
}

BOOST_AUTO_TEST_CASE(trying_to_create_empty_table)
{
  HTable table(0);
  BOOST_TEST(table.getCapacity() == 1);
}

BOOST_AUTO_TEST_CASE(add_elems_by_same_key)
{
  HTable table;
  table.add(1, 1);
  table.add(1, 2);
  BOOST_TEST(table.getSize() == 1);
  BOOST_TEST(table.get(1) == 1);
}
