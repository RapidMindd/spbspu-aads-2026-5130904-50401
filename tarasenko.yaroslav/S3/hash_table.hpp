#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <functional>
#include <stdexcept>
#include "bidir_list.hpp"
#include "vector.hpp"
#include "hmac_hash.hpp"

namespace tarasenko
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTableForwardIterator;

  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class HashTableForwardIterator< Key, Value, Hash, Equal >;
  public:
    HashTable(size_t slots = 64);

    void add(const Key& key, const Value& val);
    bool drop(const Key& key);
    const Value& get(const Key& key) const;
    bool has(const Key& key) const;
    void rehash(size_t slots);

    size_t getSize() const;
    size_t getCapacity() const;

    void swap(HashTable< Key, Value, Hash, Equal >& rhs) noexcept;

    HashTableForwardIterator< Key, Value, Hash, Equal > begin();
    HashTableForwardIterator< Key, Value, Hash, Equal > end();

  private:
    Vector< BidirList< std::pair< Key, Value > > > table_;
    size_t size_ = 0;
    Hash hash_;
    Equal equal_;
  };

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTable< Key, Value, Hash, Equal >& lhs,
    const HashTable< Key, Value, Hash, Equal >& rhs);

  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  class HashTableForwardIterator
  {
    friend class HashTable< Key, Value, Hash, Equal >;
  public:
    HashTableForwardIterator();

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    HashTableForwardIterator& operator++();
    HashTableForwardIterator operator++(int);

    bool operator==(const HashTableForwardIterator& rhs) const;
    bool operator!=(const HashTableForwardIterator& rhs) const;

  private:
    HashTable< Key, Value, Hash, Equal >* owner_;
    VecIt< BidirList< std::pair< Key, Value > > > bucketIt_;
    ListIter<std::pair< Key, Value > > listIt_;

    HashTableForwardIterator(HashTable< Key, Value, Hash, Equal >* owner,
      VecIt< BidirList< std::pair< Key, Value > > > bucketIt,
      ListIter<std::pair< Key, Value > > listIt);
  };

  #define ht_template template< class Key, class Value, class Hash, class Equal >
  #define ht_type HashTable< Key, Value, Hash, Equal >
  #define ht_iterator HashTableForwardIterator< Key, Value, Hash, Equal >

  template< class Key, class Value >
  using Bucket = BidirList< std::pair< Key, Value > >;
  template< class Key, class Value >
  using Pair = std::pair< Key, Value >;

  ht_template
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
    table_(slots > 0 ? slots : 1, Bucket< Key, Value >())
  {}

  ht_template
  void ht_type::add(const Key& key, const Value& val)
  {
    size_t slot = hash_(key) % table_.getSize();
    const auto& list = table_[slot];
    auto it = list.begin();
    while (it != list.end())
    {
      if (equal_(it->first, key))
      {
        return;
      }
      ++it;
    }
    table_[slot].push_front({key, val});
    ++size_;
  }

  ht_template
  bool ht_type::drop(const Key& key)
  {
    size_t slot = hash_(key) % table_.getSize();
    auto& list = table_[slot];
    auto it = list.begin();
    while (it != list.end())
    {
      if (equal_(it->first, key))
      {
        list.erase(it);
        --size_;
        return true;
      }
      ++it;
    }
    return false;
  }

  ht_template
  const Value& ht_type::get(const Key& key) const
  {
    size_t slot = hash_(key) % table_.getSize();
    const auto& list = table_[slot];
    auto it = list.begin();
    while (it != list.end())
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
      ++it;
    }
    throw std::runtime_error("Key not found");
  }

  ht_template
  bool ht_type::has(const Key& key) const
  {
    size_t slot = hash_(key) % table_.getSize();
    const auto& list = table_[slot];
    auto it = list.begin();
    while (it != list.end())
    {
      if (equal_(it->first, key))
      {
        return true;
      }
      ++it;
    }
    return false;
  }

  ht_template
  size_t ht_type::getSize() const
  {
    return size_;
  }

  ht_template
  size_t ht_type::getCapacity() const
  {
    return table_.getSize();
  }

  ht_template
  void ht_type::rehash(size_t slots)
  {
    ht_type copy(slots);
    for (size_t i = 0; i < getCapacity(); ++i)
    {
      const auto& list = table_[i];
      auto it = list.begin();
      while (it != list.end())
      {
        copy.add(it->first, it->second);
        ++it;
      }
    }
    swap(copy);
  }

  ht_template
  void ht_type::swap(ht_type& rhs) noexcept
  {
    table_.swap(rhs.table_);
    std::swap(size_, rhs.size_);
    std::swap(hash_, rhs.hash_);
    std::swap(equal_, rhs.equal_);
  }

  ht_template
  bool operator==(const ht_type& lhs, const ht_type& rhs)
  {
    if (lhs.getSize() != rhs.getSize())
    {
      return false;
    }
    return true;
  }

  ht_template
  Pair< Key, Value >& ht_iterator::operator*() const
  {
    return *listIt_;
  }

  ht_template
  Pair< Key, Value >* ht_iterator::operator->() const
  {
    return &(**this);
  }

  ht_template
  ht_iterator& ht_iterator::operator++()
  {
    ++listIt_;
    if (listIt_ == bucketIt_->end())
    {
      ++bucketIt_;
      while (bucketIt_ != owner_->table_.end() && bucketIt_->empty())
      {
        ++bucketIt_;
      }
      if (bucketIt_ != owner_->table_.end())
      {
        listIt_ = bucketIt_->begin();
      }
    }
    return *this;
  }

  ht_template
  ht_iterator ht_iterator::operator++(int)
  {
    ht_iterator copy = *this;
    ++(*this);
    return copy;
  }

  ht_template
  bool ht_iterator::operator==(const ht_iterator& rhs) const
  {
    if (owner_ != rhs.owner_ || bucketIt_ != rhs.bucketIt_)
    {
      return false;
    }
    if (bucketIt_ == owner_->table_.end())
    {
      return true;
    }
    return listIt_ == rhs.listIt_;
  }

  ht_template
  bool ht_iterator::operator!=(const ht_iterator& rhs) const
  {
    return !(*this == rhs);
  }

  ht_template
  ht_iterator::HashTableForwardIterator():
    owner_(nullptr)
  {}

  ht_template
  ht_iterator::HashTableForwardIterator(ht_type* owner,
    VecIt< BidirList< std::pair< Key, Value > > > bucketIt,
    ListIter<std::pair< Key, Value > > listIt):
    owner_(owner),
    bucketIt_(bucketIt),
    listIt_(listIt)
  {}

  ht_template
  ht_iterator ht_type::begin()
  {
    auto bucketIt = table_.begin();
    while (bucketIt != table_.end() && bucketIt->empty())
    {
      ++bucketIt;
    }
    if (bucketIt == table_.end())
    {
      return end();
    }

    return ht_iterator(this, bucketIt, bucketIt->begin());
  }

  ht_template
  ht_iterator ht_type::end()
  {
    return ht_iterator(this, table_.end(), ListIter< Pair< Key, Value > >());
  }

  #undef ht_template
  #undef ht_type
  #undef ht_iterator
};

#endif
