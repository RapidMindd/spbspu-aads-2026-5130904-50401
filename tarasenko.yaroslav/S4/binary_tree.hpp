#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <functional>

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

    template< class Key, class Value >
    Node< Key, Value >* next(Node< Key, Value >* node);
    template< class Key, class Value >
    Node< Key, Value >* prev(Node< Key, Value >* node);
    template< class Key, class Value >
    Node< Key, Value >* fallLeft(Node< Key, Value >* node);
    template< class Key, class Value >
    Node< Key, Value >* fallRight(Node< Key, Value >* node);
    template< class Key, class Value >
    size_t height(Node< Key, Value >* node);
  }

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    BSTree();
    BSTree(std::initializer_list< std::pair< Key, Value > > list);
    ~BSTree();

    BSTree(const BSTree< Key, Value, Compare >& rhs);
    BSTree(BSTree< Key, Value, Compare >&& rhs) noexcept;

    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >& rhs);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&& rhs) noexcept;

    bool add(const Key& key, const Value& val);
    bool drop(const Key& key);
    const Value& get(const Key& key) const;
    Value& get(const Key& key);
    bool has(const Key& key) const;

    void clear();

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
    void clear(detail::Node< Key, Value >* node);
    detail::Node< Key, Value >* copy(detail::Node< Key, Value >* node, detail::Node< Key, Value >* parent);
    void swap(BSTree< Key, Value, Compare >& rhs) noexcept;
  };

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    BSTIterator();

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

  private:
    friend class BSTree< Key, Value >;
    friend class BSTConstIterator< Key, Value >;
    BSTIterator(detail::Node< Key, Value >* node, detail::Node< Key, Value >* root) noexcept;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    BSTConstIterator();
    BSTConstIterator(const BSTIterator< Key, Value >& it);

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

  private:
    friend class BSTree< Key, Value >;
    BSTConstIterator(detail::Node< Key, Value >* node, detail::Node< Key, Value >* root) noexcept;
  };

  #define tree_template template< class Key, class Value, class Compare >
  #define tree_type BSTree< Key, Value, Compare >
  #define tree_iterator BSTIterator< Key, Value >
  #define tree_const_iterator BSTConstIterator< Key, Value >
  #define tree_node detail::Node< Key, Value >
  #define iter_template template< class Key, class Value >

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
    else if (node == node->parent_->left_)
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
      --size_;
      return true;
    }
    if (!node->right_)
    {
      replace(node, node->left_);
      delete node;
      --size_;
      return true;
    }
    tree_node* next = detail::fallLeft(node->right_);
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
    --size_;
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

  tree_template
  tree_type::~BSTree()
  {
    clear();
  }

  tree_template
  void tree_type::clear()
  {
    clear(root_);
    root_ = nullptr;
    size_ = 0;
  }

  tree_template
  void tree_type::clear(tree_node* node)
  {
    if (node)
    {
      clear(node->left_);
      clear(node->right_);
      delete node;
    }
  }

  tree_template
  tree_node* tree_type::copy(tree_node* node, tree_node* parent)
  {
    if (!node)
    {
      return nullptr;
    }
    tree_node* cur = new tree_node{node->data_, nullptr, nullptr, parent};
    try
    {
      cur->left_ = copy(node->left_, cur);
      cur->right_ = copy(node->right_, cur);
    }
    catch (...)
    {
      clear(cur);
      throw;
    }

    return cur;
  }

  tree_template
  tree_type::BSTree(const BSTree< Key, Value, Compare >& rhs):
    root_(copy(rhs.root_, nullptr)),
    size_(rhs.size_),
    comp_(rhs.comp_)
  {}

  tree_template
  tree_type::BSTree(BSTree< Key, Value, Compare >&& rhs) noexcept:
    root_(rhs.root_),
    size_(rhs.size_),
    comp_(rhs.comp_)
  {
    rhs.root_ = nullptr;
    rhs.size_ = 0;
  }

  tree_template
  void tree_type::swap(BSTree< Key, Value, Compare >& rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(size_, rhs.size_);
    std::swap(comp_, rhs.comp_);
  }

  tree_template
  tree_type& tree_type::operator=(const BSTree< Key, Value, Compare >& rhs)
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    tree_type copy = rhs;
    swap(copy);

    return *this;
  }

  tree_template
  tree_type& tree_type::operator=(BSTree< Key, Value, Compare >&& rhs) noexcept
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    swap(rhs);

    return *this;
  }

  tree_template
  tree_type::BSTree(std::initializer_list< std::pair< Key, Value > > list):
    BSTree()
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      add(it->first, it->second);
    }
  }

  iter_template
  tree_iterator::BSTIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  iter_template
  tree_const_iterator::BSTConstIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  iter_template
  tree_iterator::BSTIterator(tree_node* node, tree_node* root) noexcept:
    node_(node),
    root_(root)
  {}

  iter_template
  tree_const_iterator::BSTConstIterator(tree_node* node, tree_node* root) noexcept:
    node_(node),
    root_(root)
  {}

  iter_template
  std::pair< Key, Value >& tree_iterator::operator*() const
  {
    return node_->data_;
  }

  iter_template
  const std::pair< Key, Value >& tree_const_iterator::operator*() const
  {
    return node_->data_;
  }

  iter_template
  std::pair< Key, Value >* tree_iterator::operator->() const
  {
    return &node_->data_;
  }

  iter_template
  const std::pair< Key, Value >* tree_const_iterator::operator->() const
  {
    return &node_->data_;
  }

  template< class Key, class Value >
  tree_node* detail::fallLeft(tree_node* node)
  {
    while (node && node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  template< class Key, class Value >
  tree_node* detail::next(tree_node* node)
  {
    if (node->right_)
    {
      return fallLeft(node->right_);
    }
    else
    {
      while (node->parent_ && node->parent_->right_ == node)
      {
        node = node->parent_;
      }
      return node->parent_;
    }
  }

  template< class Key, class Value >
  tree_node* detail::fallRight(tree_node* node)
  {
    while (node && node->right_)
    {
      node = node->right_;
    }
    return node;
  }

  template< class Key, class Value >
  tree_node* detail::prev(tree_node* node)
  {
    if (node->left_)
    {
      return fallRight(node->left_);
    }
    else
    {
      while (node->parent_ && node->parent_->left_ == node)
      {
        node = node->parent_;
      }
      return node->parent_;
    }
  }

  iter_template
  tree_iterator& tree_iterator::operator++()
  {
    node_ = detail::next(node_);
    return *this;
  }

  iter_template
  tree_const_iterator& tree_const_iterator::operator++()
  {
    node_ = detail::next(node_);
    return *this;
  }

  iter_template
  tree_iterator tree_iterator::operator++(int)
  {
    tree_iterator copy = *this;
    ++(*this);
    return copy;
  }

  iter_template
  tree_const_iterator tree_const_iterator::operator++(int)
  {
    tree_const_iterator copy = *this;
    ++(*this);
    return copy;
  }

  iter_template
  tree_iterator& tree_iterator::operator--()
  {
    if (!node_)
    {
      node_ = detail::fallRight(root_);
      return *this;
    }
    node_ = detail::prev(node_);
    return *this;
  }

  iter_template
  tree_const_iterator& tree_const_iterator::operator--()
  {
    if (!node_)
    {
      node_ = detail::fallRight(root_);
      return *this;
    }
    node_ = detail::prev(node_);
    return *this;
  }

  iter_template
  tree_iterator tree_iterator::operator--(int)
  {
    tree_iterator copy = *this;
    --(*this);
    return copy;
  }

  iter_template
  tree_const_iterator tree_const_iterator::operator--(int)
  {
    tree_const_iterator copy = *this;
    --(*this);
    return copy;
  }

  iter_template
  bool tree_iterator::operator==(const tree_iterator& rhs) const
  {
    return node_ == rhs.node_;
  }

  iter_template
  bool tree_const_iterator::operator==(const tree_const_iterator& rhs) const
  {
    return node_ == rhs.node_;
  }

  iter_template
  bool tree_iterator::operator!=(const tree_iterator& rhs) const
  {
    return !(*this == rhs);
  }

  iter_template
  bool tree_const_iterator::operator!=(const tree_const_iterator& rhs) const
  {
    return !(*this == rhs);
  }

  tree_template
  tree_iterator tree_type::begin()
  {
    return tree_iterator(detail::fallLeft(root_), root_);
  }

  tree_template
  tree_const_iterator tree_type::begin() const
  {
    return tree_const_iterator(detail::fallLeft(root_), root_);
  }

  tree_template
  tree_iterator tree_type::end()
  {
    return tree_iterator(nullptr, root_);
  }

  tree_template
  tree_const_iterator tree_type::end() const
  {
    return tree_const_iterator(nullptr, root_);
  }

  tree_template
  tree_const_iterator tree_type::cbegin() const
  {
    return tree_const_iterator(detail::fallLeft(root_), root_);
  }

  tree_template
  tree_const_iterator tree_type::cend() const
  {
    return tree_const_iterator(nullptr, root_);
  }

  template< class Key, class Value >
  size_t detail::height(tree_node* node)
  {
    if (!node)
    {
      return 0;
    }
    return std::max(height(node->left_), height(node->right_)) + 1;
  }

  tree_template
  size_t tree_type::height(tree_const_iterator it) const
  {
    return detail::height(it.node_);
  }

  tree_template
  size_t tree_type::height() const
  {
    return detail::height(root_);
  }

  iter_template
  tree_const_iterator::BSTConstIterator(const tree_iterator& it):
    node_(it.node_),
    root_(it.root_)
  {}

  tree_template
  tree_const_iterator tree_type::rotateLeft(tree_const_iterator it)
  {
    detail::Node< Key, Value >* p = it.node_;
    detail::Node< Key, Value >* q = p->parent_;
    q->right_ = p->left_;
    if (p->left_)
    {
      p->left_->parent_ = q;
    }
    p->parent_ = q->parent_;
    if (!q->parent_)
    {
      root_ = p;
    }
    else if (q == q->parent_->left_)
    {
      q->parent_->left_ = p;
    }
    else
    {
      q->parent_->right_ = p;
    }
    q->parent_ = p;
    p->left_ = q;
    return tree_const_iterator(p, root_);
  }

  tree_template
  tree_const_iterator tree_type::rotateRight(tree_const_iterator it)
  {
    detail::Node< Key, Value >* p = it.node_;
    detail::Node< Key, Value >* q = p->parent_;
    q->left_ = p->right_;
    if (p->right_)
    {
      p->right_->parent_ = q;
    }
    p->parent_ = q->parent_;
    if (!q->parent_)
    {
      root_ = p;
    }
    else if (q == q->parent_->left_)
    {
      q->parent_->left_ = p;
    }
    else
    {
      q->parent_->right_ = p;
    }
    q->parent_ = p;
    p->right_ = q;
    return tree_const_iterator(p, root_);
  }

  tree_template
  tree_const_iterator tree_type::rotateLargeLeft(tree_const_iterator it)
  {
    it = rotateRight(it);
    return rotateLeft(it);
  }

  tree_template
  tree_const_iterator tree_type::rotateLargeRight(tree_const_iterator it)
  {
    it = rotateLeft(it);
    return rotateRight(it);
  }

  #undef tree_template
  #undef tree_type
  #undef tree_iterator
  #undef tree_const_iterator
  #undef tree_node
  #undef iter_template
}

#endif
