#include <iostream>
#include "bidir_list.hpp"

int main()
{
  using stringListPair = std::pair< std::string, tarasenko::BidirList< size_t > >;
  auto list = tarasenko::BidirList< stringListPair >();
  std::string current_str;

  while (std::cin >> current_str)
  {
    auto current_list = tarasenko::BidirList< size_t >();
    size_t current_val = 0;
    while (std::cin >> current_val)
    {
      current_list.push_back(current_val);
    }
    list.push_back(std::make_pair(current_str, current_list));
    std::cin.clear();
  }

  if (list.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  auto temp = list.begin()->first;
  std::cout << temp;
  for (auto it = ++(list.begin()); it != list.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";

  size_t max_size = 0;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    max_size = std::max(max_size, it->second.size());
  }

  using IterSizePair = std::pair< tarasenko::ListIter< size_t >, size_t >;
  auto iter_list = tarasenko::BidirList< IterSizePair >();
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    iter_list.push_back(std::make_pair(it->second.begin(), it->second.size()));
  }

  for (size_t i = 0; i < max_size; ++i)
  {
    bool flag = true;
    for (auto it = iter_list.begin(); it != iter_list.end(); ++it)
    {
      if ((*it).second > i)
      {
        if (flag)
        {
          std::cout << *((*it).first);
          ++((*it).first);
          flag = false;
        }
        else
        {
          std::cout << " " << *((*it).first);
          ++((*it).first);
        }
      }
    }
    std::cout << "\n";
  }
}
