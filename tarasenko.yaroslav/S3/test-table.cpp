#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"

using namespace tarasenko;

BOOST_AUTO_TEST_CASE(default_constructor)
{
  HashTable< int, int, std::hash< int >, std::equal_to< int > > table;
  BOOST_TEST(table.getSize() == 0);
}
