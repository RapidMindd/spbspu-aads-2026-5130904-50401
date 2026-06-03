#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <cstddef>
#include "less_to.hpp"
#include "utility"

namespace tarasenko
{
  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value >
  class BSTConstIterator;

  namespace detail
  {
    template< class Key, class Value >
    struct Node
    {
      std::pair< Key, Value > data_;
      Node< Key, Value >* left_;
      Node< Key, Value >* right_;
      Node< Key, Value >* parent_;
    };
  }

  template< class Key, class Value, class Compare = LessTo< Key > >
  class BSTree
  {
  public:
    void add(const Key& key, const Value& val);
    bool drop(const Key& key);
    const Value& get(const Key& key) const;
    Value& get(const Key& key);
    bool has(const Key& key) const;

    size_t getSize() const;
    bool isEmpty() const;

    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    size_t height(const_iterator it) const;
    size_t height() const;

  private:
    detail::Node< Key, Value >* root_;
    size_t size_;
    Compare comp_;
  };

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    BSTIterator& operator++();
    BSTIterator operator++(int);

    BSTIterator& operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator& it) const;
    bool operator!=(const BSTIterator& it) const;

  private:
    detail::Node< Key, Value >* node_ = nullptr;
    detail::Node< Key, Value >* root_ = nullptr;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);

    BSTConstIterator& operator--();
    BSTConstIterator operator--(int);

    bool operator==(const BSTConstIterator& it) const;
    bool operator!=(const BSTConstIterator& it) const;

  private:
    detail::Node< Key, Value >* node_ = nullptr;
    detail::Node< Key, Value >* root_ = nullptr;
  };
}

#endif
