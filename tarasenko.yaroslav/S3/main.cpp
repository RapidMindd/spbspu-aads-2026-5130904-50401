#include <iostream>
#include <string>
#include <algorithm>
#include <cstddef>
#include <exception>
#include "hash_table.hpp"
#include "graph.hpp"

using namespace tarasenko;
using HTable = HashTable< std::string, Graph >;

void showGraphs(std::istream&, std::ostream& out, HTable& graphs)
{
  if (graphs.isEmpty())
  {
    out << "\n";
    return;
  }
  Vector< std::string > graphNames;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    graphNames.pushBack(it->first);
  }
  std::sort(graphNames.begin(), graphNames.end());

  for (auto it = graphNames.begin(); it != graphNames.end(); ++it)
  {
    out << *it << "\n";
  }
}

void showVertexes(std::istream& in, std::ostream& out, HTable& graphs)
{
  std::string graphName;
  in >> graphName;
  if (graphs.has(graphName))
  {
    Vector< std::string > vertexes = graphs.get(graphName).getVertexes();
    if (vertexes.isEmpty())
    {
      out << "\n";
      return;
    }
    std::sort(vertexes.begin(), vertexes.end());
    for (auto it = vertexes.begin(); it != vertexes.end(); ++it)
    {
      out << *it << "\n";
    }
  }
  else
  {
    throw std::runtime_error("No graph with this name");
  }
}

void showOutbound(std::istream& in, std::ostream& out, HTable& graphs)
{
  std::string graphName;
  in >> graphName;
  std::string vertexName;
  in >> vertexName;
  if (graphs.has(graphName) && graphs.get(graphName).hasVertex(vertexName))
  {
    Vector< EdgeInfo > outbound = graphs.get(graphName).getOutbound(vertexName);
    if (outbound.isEmpty())
    {
      out << "\n";
      return;
    }
    for (auto it = outbound.begin(); it != outbound.end(); ++it)
    {
      out << it->vertex;
      for (auto weightsIt = it->weights.begin(); weightsIt != it->weights.end(); ++weightsIt)
      {
        out << " " << *weightsIt;
      }
      out << "\n";
    }
  }
  else
  {
    throw std::runtime_error("No graph or vertex with this name");
  }
}

void showInbound(std::istream& in, std::ostream& out, HTable& graphs)
{
  std::string graphName;
  in >> graphName;
  std::string vertexName;
  in >> vertexName;
  if (graphs.has(graphName) && graphs.get(graphName).hasVertex(vertexName))
  {
    Vector< EdgeInfo > inbound = graphs.get(graphName).getInbound(vertexName);
    if (inbound.isEmpty())
    {
      out << "\n";
      return;
    }
    for (auto it = inbound.begin(); it != inbound.end(); ++it)
    {
      out << it->vertex;
      for (auto weightsIt = it->weights.begin(); weightsIt != it->weights.end(); ++weightsIt)
      {
        out << " " << *weightsIt;
      }
      out << "\n";
    }
  }
  else
  {
    throw std::runtime_error("No graph or vertex with this name");
  }
}

void bind(std::istream& in, std::ostream&, HTable& graphs)
{
  std::string graphName;
  in >> graphName;
  std::string from;
  in >> from;
  std::string to;
  in >> to;
  unsigned int weight;
  in >> weight;
  if (graphs.has(graphName))
  {
    graphs.get(graphName).bind(from, to, weight);
  }
  else
  {
    throw std::runtime_error("No graph with this name");
  }
}

void cut(std::istream& in, std::ostream&, HTable& graphs)
{
  std::string graphName;
  in >> graphName;
  std::string from;
  in >> from;
  std::string to;
  in >> to;
  unsigned int weight;
  in >> weight;
  if (graphs.has(graphName))
  {
    graphs.get(graphName).cut(from, to, weight);
  }
  else
  {
    throw std::runtime_error("No graph with this name");
  }
}

void createGraph(std::istream& in, std::ostream&, HTable& graphs)
{
  std::string graphName;
  in >> graphName;
  if (!graphs.has(graphName))
  {
    size_t vertexesCount;
    in >> vertexesCount;
    Graph graph;
    for (size_t i = 0; i < vertexesCount; ++i)
    {
      std::string vertexName;
      in >> vertexName;
      graph.addVertex(vertexName);
    }
    graphs.add(graphName, graph);
  }
  else
  {
    throw std::runtime_error("Graph with this name already exists");
  }
}

void mergeGraphs(std::istream& in, std::ostream&, HTable& graphs)
{
  std::string resultGraphName;
  in >> resultGraphName;
  std::string firstGraphName;
  in >> firstGraphName;
  std::string secondGraphName;
  in >> secondGraphName;
  if (graphs.has(firstGraphName) && graphs.has(secondGraphName) && !graphs.has(resultGraphName))
  {
    Graph result = merge(graphs.get(firstGraphName), graphs.get(secondGraphName));
    graphs.add(resultGraphName, result);
  }
  else
  {
    throw std::runtime_error("No graph with this name or result graph already exists");
  }
}

void extractGraph(std::istream& in, std::ostream&, HTable& graphs)
{
  std::string resultGraphName;
  in >> resultGraphName;
  std::string graphName;
  in >> graphName;
  if (graphs.has(graphName) && !graphs.has(resultGraphName))
  {
    size_t vertexesCount;
    in >> vertexesCount;
    Vector< std::string > vertexes;
    for (size_t i = 0; i < vertexesCount; ++i)
    {
      std::string vertexName;
      in >> vertexName;
      vertexes.pushBack(vertexName);
    }
    Graph result = graphs.get(graphName).extract(vertexes);
    graphs.add(resultGraphName, result);
  }
  else
  {
    throw std::runtime_error("No graph with this name or result graph already exists");
  }
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "1 argument expected\n";
    return 1;
  }

  HTable graphs = getFromFile(argv[1]);
  using cmd_t = void(*)(std::istream&, std::ostream&, HTable&);
  HashTable< std::string, cmd_t > cmds;
  cmds.add("graphs", showGraphs);
  cmds.add("vertexes", showVertexes);
  cmds.add("outbound", showOutbound);
  cmds.add("inbound", showInbound);
  cmds.add("bind", bind);
  cmds.add("cut", cut);
  cmds.add("create", createGraph);
  cmds.add("merge", mergeGraphs);
  cmds.add("extract", extractGraph);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.get(cmd)(std::cin, std::cout, graphs);
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
