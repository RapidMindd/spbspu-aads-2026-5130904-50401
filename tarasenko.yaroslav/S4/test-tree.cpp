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

BOOST_AUTO_TEST_CASE(dereference)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  BOOST_TEST(tree.begin()->first == 1);
  BOOST_TEST(tree.cbegin()->first == 1);
  BOOST_CHECK(*(tree.begin()) == std::make_pair(1, 1));
  BOOST_CHECK(*(tree.cbegin()) == std::make_pair(1, 1));
}

BOOST_AUTO_TEST_CASE(forward_iteratrion)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  int i = 1;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    BOOST_TEST(it->first == i);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(const_forward_iteratrion)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  const Tree ctree = tree;
  int i = 1;
  for (auto it = ctree.begin(); it != ctree.end(); ++it)
  {
    BOOST_TEST(it->first == i);
    ++i;
  }
  i = 1;
  for (auto it = ctree.cbegin(); it != ctree.cend(); ++it)
  {
    BOOST_TEST(it->first == i);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(backward_iteratrion)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  int i = 4;
  for (auto it = tree.end(); it != tree.begin();)
  {
    --it;
    BOOST_TEST(it->first == i);
    --i;
  }
}

BOOST_AUTO_TEST_CASE(const_backward_iteratrion)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  const Tree ctree = tree;
  int i = 4;
  for (auto it = ctree.end(); it != ctree.begin();)
  {
    --it;
    BOOST_TEST(it->first == i);
    --i;
  }
  i = 4;
  for (auto it = ctree.cend(); it != ctree.cbegin();)
  {
    --it;
    BOOST_TEST(it->first == i);
    --i;
  }
}

BOOST_AUTO_TEST_CASE(increment_and_decrement)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  auto it = tree.begin();
  ++it;
  BOOST_TEST(it->first == 2);
  --it;
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it++->first == 1);
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it--->first == 2);
  BOOST_TEST(it->first == 1);
}

BOOST_AUTO_TEST_CASE(const_increment_and_decrement)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  const Tree ctree = tree;
  auto it = ctree.begin();
  ++it;
  BOOST_TEST(it->first == 2);
  --it;
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it++->first == 1);
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it--->first == 2);
  BOOST_TEST(it->first == 1);
}

BOOST_AUTO_TEST_CASE(height)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  BOOST_TEST(tree.height() == 3);
  auto it = tree.begin();
  BOOST_TEST(tree.height(tree.cbegin()) == 1);
  BOOST_TEST(tree.height(it) == 1);
  BOOST_TEST(tree.height(++it) == 3);
  BOOST_TEST(tree.height(++it) == 1);
  BOOST_TEST(tree.height(++it) == 2);
}

BOOST_AUTO_TEST_CASE(rotate_left)
{
  Tree tree = {{2, 2}, {1, 1}, {3, 3}};
  auto it = tree.begin();
  ++++it;
  auto root = tree.rotateLeft(it);
  BOOST_TEST(root->first == 3);
  BOOST_TEST(tree.height(root) == 3);
  BOOST_TEST(tree.height() == 3);
  BOOST_TEST(tree.height(++tree.begin()) == 2);
  BOOST_TEST(tree.getSize() == 3);
  int i = 1;
  for (auto pos = tree.begin(); pos != tree.end(); ++pos)
  {
    BOOST_TEST(pos->first == i);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(rotate_right)
{
  Tree tree = {{2, 2}, {1, 1}, {3, 3}};
  auto root = tree.rotateRight(tree.begin());
  BOOST_TEST(root->first == 1);
  BOOST_TEST(tree.height(root) == 3);
  BOOST_TEST(tree.height() == 3);
  BOOST_TEST(tree.height(++tree.begin()) == 2);
  BOOST_TEST(tree.getSize() == 3);
  int i = 1;
  for (auto pos = tree.begin(); pos != tree.end(); ++pos)
  {
    BOOST_TEST(pos->first == i);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
  Tree tree = {{2, 2}, {1, 1}, {4, 4}, {3, 3}};
  auto it = tree.begin();
  ++++it;
  auto root = tree.rotateLargeLeft(it);
  BOOST_TEST(root->first == 3);
  BOOST_TEST(tree.height(root) == 3);
  BOOST_TEST(tree.height() == 3);
  BOOST_TEST(tree.height(++tree.begin()) == 2);
  BOOST_TEST(tree.getSize() == 4);
  int i = 1;
  for (auto pos = tree.begin(); pos != tree.end(); ++pos)
  {
    BOOST_TEST(pos->first == i);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
  Tree tree = {{4, 4}, {2, 2}, {5, 5}, {3, 3}};
  auto it = tree.begin();
  ++it;
  auto root = tree.rotateLargeRight(it);
  BOOST_TEST(root->first == 3);
  BOOST_TEST(tree.height(root) == 3);
  BOOST_TEST(tree.height() == 3);
  BOOST_TEST(tree.height(++tree.begin()) == 3);
  BOOST_TEST(tree.getSize() == 4);
  int i = 2;
  for (auto pos = tree.begin(); pos != tree.end(); ++pos)
  {
    BOOST_TEST(pos->first == i);
    ++i;
  }
}
