#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include "binary_tree.hpp"

using namespace tarasenko;

using Dataset = BSTree< int, std::string >;
using Datasets = BSTree< std::string, Dataset >;

std::string readSubstr(const std::string& line, size_t& pos)
{
  size_t end = line.find(' ', pos);
  if (end == std::string::npos)
  {
    std::string substr = line.substr(pos);
    pos = line.size();
    return substr;
  }
  std::string substr = line.substr(pos, end - pos);
  pos = end + 1;
  return substr;
}

Datasets getFromFile(const std::string& filename)
{
  std::ifstream input(filename);
  if (!input)
  {
    throw std::runtime_error("Failed to open file");
  }

  Datasets datasets;
  std::string line;
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }
    size_t pos = 0;
    std::string name = readSubstr(line, pos);
    if (name.empty())
    {
      throw std::runtime_error("Incorrect dataset description");
    }

    Dataset dataset;
    while (pos < line.size())
    {
      std::string key = readSubstr(line, pos);
      std::string value = readSubstr(line, pos);
      if (key.empty() || value.empty())
      {
        throw std::runtime_error("Incorrect dataset description");
      }
      size_t read = 0;
      int keyNumber = std::stoi(key, &read);
      if (read != key.size())
      {
        throw std::runtime_error("Incorrect dataset description");
      }
      dataset.add(keyNumber, value);
    }

    datasets.add(name, dataset);
  }

  return datasets;
}

void print(std::istream& in, std::ostream& out, Datasets& datasets)
{
  std::string name;
  in >> name;
  if (!in || !datasets.has(name))
  {
    throw std::runtime_error("Invalid command");
  }

  Dataset& dataset = datasets.at(name);
  if (dataset.isEmpty())
  {
    out << "<EMPTY>\n";
    return;
  }
  out << name;
  for (auto it = dataset.begin(); it != dataset.end(); ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
  out << '\n';
}

void complement(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;
  in >> newName >> lhsName >> rhsName;
  if (!in || !datasets.has(lhsName) || !datasets.has(rhsName))
  {
    throw std::runtime_error("Invalid command");
  }

  const Dataset& lhs = datasets.at(lhsName);
  const Dataset& rhs = datasets.at(rhsName);
  Dataset result;
  for (auto it = lhs.begin(); it != lhs.end(); ++it)
  {
    if (!rhs.has(it->first))
    {
      result.add(it->first, it->second);
    }
  }
  if (datasets.has(newName))
  {
    datasets.at(newName) = result;
  }
  else
  {
    datasets.add(newName, result);
  }
}

void intersect(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;
  in >> newName >> lhsName >> rhsName;
  if (!in || !datasets.has(lhsName) || !datasets.has(rhsName))
  {
    throw std::runtime_error("Invalid command");
  }

  const Dataset& lhs = datasets.at(lhsName);
  const Dataset& rhs = datasets.at(rhsName);
  Dataset result;
  for (auto it = lhs.begin(); it != lhs.end(); ++it)
  {
    if (rhs.has(it->first))
    {
      result.add(it->first, it->second);
    }
  }
  if (datasets.has(newName))
  {
    datasets.at(newName) = result;
  }
  else
  {
    datasets.add(newName, result);
  }
}

void Union(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;
  in >> newName >> lhsName >> rhsName;
  if (!in || !datasets.has(lhsName) || !datasets.has(rhsName))
  {
    throw std::runtime_error("Invalid command");
  }

  const Dataset& lhs = datasets.at(lhsName);
  const Dataset& rhs = datasets.at(rhsName);
  Dataset result;
  for (auto it = lhs.begin(); it != lhs.end(); ++it)
  {
    result.add(it->first, it->second);
  }
  for (auto it = rhs.begin(); it != rhs.end(); ++it)
  {
    result.add(it->first, it->second);
  }
  if (datasets.has(newName))
  {
    datasets.at(newName) = result;
  }
  else
  {
    datasets.add(newName, result);
  }
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "1 argument expected\n";
    return 1;
  }

  Datasets datasets = getFromFile(argv[1]);
  using cmd_t = void(*)(std::istream&, std::ostream&, Datasets&);
  BSTree< std::string, cmd_t > cmds;
  cmds.add("print", print);
  cmds.add("complement", complement);
  cmds.add("intersect", intersect);
  cmds.add("union", Union);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, datasets);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.clear();
      std::cin.ignore(toignore, '\n');
    }
  }
}
