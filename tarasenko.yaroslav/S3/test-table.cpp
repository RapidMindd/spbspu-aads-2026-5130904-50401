#include <boost/test/unit_test.hpp>
#include <string>
#include "hash_table.hpp"

using namespace tarasenko;

using HTable = HashTable< int, int >;

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

BOOST_AUTO_TEST_CASE(swap)
{
  HTable table1;
  HTable table2;
  table1.add(1, 1);
  table2.add(2, 2);
  table1.swap(table2);
  BOOST_TEST(table1.get(2) == 2);
  BOOST_TEST(table2.get(1) == 1);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2(table1);
  BOOST_TEST(table2.get(1) == 1);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2(std::move(table1));
  BOOST_TEST(table2.get(1) == 1);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2;
  table2 = table1;
  BOOST_TEST(table2.get(1) == 1);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2;
  table2 = std::move(table1);
  BOOST_TEST(table2.get(1) == 1);
}

BOOST_AUTO_TEST_CASE(begin)
{
  HTable table;
  table.add(1, 2);
  BOOST_CHECK(table.begin() != table.end());
}

BOOST_AUTO_TEST_CASE(begin_empty)
{
  HTable table;
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(dereference)
{
  HTable table;
  table.add(1, 2);
  auto it = table.begin();
  BOOST_CHECK(*it == std::make_pair(1, 2));
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 2);
}

BOOST_AUTO_TEST_CASE(pre_increment)
{
  HTable table;
  table.add(1, 2);
  auto it = table.begin();
  BOOST_CHECK(++it == table.end());
}

BOOST_AUTO_TEST_CASE(post_increment)
{
  HTable table;
  table.add(1, 2);
  auto it = table.begin();
  BOOST_CHECK(it++ == table.begin());
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(cycle_by_iterators)
{
  HTable table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);
  int i = 0;
  for (auto it = table.begin(); it != table.end(); ++it, ++i)
  {
    BOOST_TEST(it->second == it->first * 10);
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(comparison_operator_for_table)
{
  HTable table1;
  HTable table2;
  for (int i = 0; i < 10; ++i)
  {
    table1.add(i, i * 67);
    table2.add(i, i * 67);
  }
  BOOST_CHECK(table1 == table2);
  table2.drop(3);
  BOOST_CHECK(table1 != table2);
  table2.add(10, 10 * 67);
  BOOST_CHECK(table1 != table2);
}
