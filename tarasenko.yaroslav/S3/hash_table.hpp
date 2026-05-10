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
  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
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

  private:
    Vector< BidirList< std::pair< Key, Value > > > table_;
    size_t size_ = 0;
    Hash hash_;
    Equal equal_;
  };

  #define ht_template template< class Key, class Value, class Hash, class Equal >
  #define ht_type HashTable< Key, Value, Hash, Equal >

  template< class Key, class Value >
  using Bucket = BidirList< std::pair< Key, Value > >;

  ht_template
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
    table_(slots, Bucket< Key, Value >())
  {}

  ht_template
  void ht_type::add(const Key& key, const Value& val)
  {
    size_t slot = hash_(key) % table_.getSize();
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

  #undef ht_template
  #undef ht_type
};

#endif
