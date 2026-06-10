#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <cstddef>
#include <stdexcept>
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
    BSTree();

    bool add(const Key& key, const Value& val);
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

  private:
    detail::Node< Key, Value >* find(const Key& key) const;
    void replace(detail::Node< Key, Value >* node, detail::Node< Key, Value >* child);
    detail::Node< Key, Value >* siftLeft(detail::Node< Key, Value >* node) const;
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

  #define tree_template template< class Key, class Value, class Compare >
  #define tree_type BSTree< Key, Value, Compare >
  #define tree_iterator BSTIterator< Key, Value >
  #define tree_const_iterator BSTConstIterator< Key, Value >
  #define tree_node detail::Node< Key, Value >

  tree_template
  bool tree_type::add(const Key& key, const Value& val)
  {
    if (!root_)
    {
      root_ = new tree_node{{key, val}, nullptr, nullptr, nullptr};
      ++size_;
      return true;
    }
    tree_node* cur = root_;
    while (cur)
    {
      if (comp_(cur->data_.first, key))
      {
        if (!cur->right_)
        {
          cur->right_ = new tree_node{{key, val}, nullptr, nullptr, cur};
          ++size_;
          return true;
        }
        cur = cur->right_;
      }
      else if (comp_(key, cur->data_.first))
      {
        if (!cur->left_)
        {
          cur->left_ = new tree_node{{key, val}, nullptr, nullptr, cur};
          ++size_;
          return true;
        }
        cur = cur->left_;
      }
      else
      {
        return false;
      }
    }
    return false;
  }

  tree_template
  tree_node* tree_type::find(const Key& key) const
  {
    tree_node* cur = root_;
    while (cur)
    {
      if (comp_(cur->data_.first, key))
      {
        cur = cur->right_;
      }
      else if (comp_(key, cur->data_.first))
      {
        cur = cur->left_;
      }
      else
      {
        return cur;
      }
    }
    return nullptr;
  }

  tree_template
  void tree_type::replace(tree_node* node, tree_node* child)
  {
    if (node->parent_ == nullptr)
    {
      root_ = child;
    }
    else if (node == node->parent->left)
    {
      node->parent_->left_ = child;
    }
    else
    {
      node->parent_->right_ = child;
    }
    if (child)
    {
      child->parent_ = node->parent_;
    }
  }

  tree_template
  tree_node* tree_type::siftLeft(tree_node* node) const
  {
    while (node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  tree_template
  bool tree_type::drop(const Key& key)
  {
    tree_node* node = find(key);
    if (!node)
    {
      return false;
    }
    if (!node->left_)
    {
      replace(node, node->right_);
      delete node;
      return true;
    }
    if (!node->right_)
    {
      replace(node, node->left_);
      delete node;
      return true;
    }
    tree_node* next = siftLeft(node->right_);
    if (next->parent_ != node)
    {
      replace(next, next->right_);
      next->right_ = node->right_;
      next->right_->parent_ = next;
    }
    replace(node, next);
    next->left_ = node->left_;
    next->left_->parent_ = next;
    delete node;
    return true;
  }

  tree_template
  tree_type::BSTree():
    root_(nullptr),
    size_(0),
    comp_(Compare{})
  {}

  tree_template
  size_t tree_type::getSize() const
  {
    return size_;
  }

  tree_template
  bool tree_type::isEmpty() const
  {
    return !size_;
  }

  tree_template
  const Value& tree_type::get(const Key& key) const
  {
    tree_node* node = find(key);
    if (!node)
    {
      throw std::runtime_error("Key not found");
    }
    return node->data_.second;
  }

  tree_template
  Value& tree_type::get(const Key& key)
  {
    return const_cast< Value& >(const_cast< const tree_type* >(this)->get(key));
  }

  tree_template
  bool tree_type::has(const Key& key) const
  {
    return find(key);
  }

  #undef tree_template
  #undef tree_type
  #undef tree_iterator
  #undef tree_const_iterator
  #undef tree_node
}

#endif
