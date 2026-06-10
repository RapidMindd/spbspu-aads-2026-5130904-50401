#include <boost/test/unit_test.hpp>
#include "binary_tree.hpp"

using namespace tarasenko;

using Tree = BSTree< int, int >;

BOOST_AUTO_TEST_CASE(default_constructor)
{
  Tree tree;
  BOOST_TEST(tree.isEmpty());
  BOOST_TEST(tree.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(add)
{
  Tree tree;
  tree.add(1, 1);
  BOOST_TEST(!tree.isEmpty());
  BOOST_TEST(tree.getSize() == 1);
  BOOST_TEST(tree.get(1) == 1);
}

BOOST_AUTO_TEST_CASE(add_existing)
{
  Tree tree;
  tree.add(1, 1);
  BOOST_TEST(!tree.add(1, 2));
  BOOST_TEST(tree.get(1) == 1);
}

BOOST_AUTO_TEST_CASE(drop)
{
  Tree tree;
  tree.add(1, 1);
  BOOST_TEST(tree.drop(1));
  BOOST_TEST(tree.isEmpty());
  BOOST_TEST(tree.getSize() == 0);
  BOOST_TEST(!tree.drop(1));
}

BOOST_AUTO_TEST_CASE(get)
{
  Tree tree;
  tree.add(1, 1);
  BOOST_TEST(tree.get(1) == 1);
  BOOST_CHECK_THROW(tree.get(2), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(const_get)
{
  Tree tree;
  tree.add(1, 1);
  const Tree ctree = tree;
  BOOST_TEST(ctree.get(1) == 1);
  BOOST_CHECK_THROW(ctree.get(2), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(has)
{
  Tree tree;
  tree.add(1, 1);
  BOOST_TEST(tree.has(1));
  BOOST_TEST(!tree.has(2));
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
  Tree tree = {{1, 1}, {2, 2}, {3, 3}};
  BOOST_TEST(tree.getSize() == 3);
  BOOST_TEST(tree.get(1) == 1);
  BOOST_TEST(tree.get(2) == 2);
  BOOST_TEST(tree.get(3) == 3);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  Tree copy = tree;
  BOOST_TEST(copy.getSize() == 4);
  for (int i = 1; i <= 4; ++i)
  {
    BOOST_TEST(copy.get(i) == i);
  }
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  Tree copy;
  copy = tree;
  BOOST_TEST(copy.getSize() == 4);
  for (int i = 1; i <= 4; ++i)
  {
    BOOST_TEST(copy.get(i) == i);
  }
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  Tree copy = std::move(tree);
  BOOST_TEST(copy.getSize() == 4);
  for (int i = 1; i <= 4; ++i)
  {
    BOOST_TEST(copy.get(i) == i);
  }
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  Tree copy;
  copy = std::move(tree);
  BOOST_TEST(copy.getSize() == 4);
  for (int i = 1; i <= 4; ++i)
  {
    BOOST_TEST(copy.get(i) == i);
  }
}
