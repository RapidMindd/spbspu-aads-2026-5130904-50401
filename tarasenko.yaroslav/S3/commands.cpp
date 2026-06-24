#include "commands.hpp"
#include <algorithm>
#include <istream>
#include <ostream>
#include <stdexcept>

void tarasenko::showGraphs(std::istream&, std::ostream& out, GraphTable& graphs)
{
  if (graphs.empty())
  {
    return;
  }
  Vector< std::string > graphNames;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    graphNames.pushBack(it->first);
  }
  std::sort(graphNames.begin(), graphNames.end());

  auto it = graphNames.begin();
  out << *it;
  for (++it; it != graphNames.end(); ++it)
  {
    out << "\n" << *it;
  }
}

void tarasenko::showVertexes(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  in >> graphName;
  if (graphs.has(graphName))
  {
    Vector< std::string > vertexes = graphs.at(graphName).getVertexes();
    if (vertexes.isEmpty())
    {
      return;
    }
    std::sort(vertexes.begin(), vertexes.end());
    auto it = vertexes.begin();
    out << *it;
    for (++it; it != vertexes.end(); ++it)
    {
      out << "\n" << *it;
    }
  }
  else
  {
    throw std::runtime_error("No graph with this name");
  }
}

void tarasenko::showOutbound(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  in >> graphName;
  std::string vertexName;
  in >> vertexName;
  if (graphs.has(graphName) && graphs.at(graphName).hasVertex(vertexName))
  {
    Vector< EdgeInfo > outbound = graphs.at(graphName).getOutbound(vertexName);
    if (outbound.isEmpty())
    {
      return;
    }
    auto it = outbound.begin();
    out << it->vertex;
    for (auto weightsIt = it->weights.begin(); weightsIt != it->weights.end(); ++weightsIt)
    {
      out << " " << *weightsIt;
    }
    for (++it; it != outbound.end(); ++it)
    {
      out << "\n" << it->vertex;
      for (auto weightsIt = it->weights.begin(); weightsIt != it->weights.end(); ++weightsIt)
      {
        out << " " << *weightsIt;
      }
    }
  }
  else
  {
    throw std::runtime_error("No graph or vertex with this name");
  }
}

void tarasenko::showInbound(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  in >> graphName;
  std::string vertexName;
  in >> vertexName;
  if (graphs.has(graphName) && graphs.at(graphName).hasVertex(vertexName))
  {
    Vector< EdgeInfo > inbound = graphs.at(graphName).getInbound(vertexName);
    if (inbound.isEmpty())
    {
      return;
    }
    auto it = inbound.begin();
    out << it->vertex;
    for (auto weightsIt = it->weights.begin(); weightsIt != it->weights.end(); ++weightsIt)
    {
      out << " " << *weightsIt;
    }
    for (++it; it != inbound.end(); ++it)
    {
      out << "\n" << it->vertex;
      for (auto weightsIt = it->weights.begin(); weightsIt != it->weights.end(); ++weightsIt)
      {
        out << " " << *weightsIt;
      }
    }
  }
  else
  {
    throw std::runtime_error("No graph or vertex with this name");
  }
}

void tarasenko::bind(std::istream& in, std::ostream&, GraphTable& graphs)
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
    graphs.at(graphName).bind(from, to, weight);
  }
  else
  {
    throw std::runtime_error("No graph with this name");
  }
}

void tarasenko::cut(std::istream& in, std::ostream&, GraphTable& graphs)
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
    graphs.at(graphName).cut(from, to, weight);
  }
  else
  {
    throw std::runtime_error("No graph with this name");
  }
}

void tarasenko::createGraph(std::istream& in, std::ostream&, GraphTable& graphs)
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

void tarasenko::mergeGraphs(std::istream& in, std::ostream&, GraphTable& graphs)
{
  std::string resultGraphName;
  in >> resultGraphName;
  std::string firstGraphName;
  in >> firstGraphName;
  std::string secondGraphName;
  in >> secondGraphName;
  if (graphs.has(firstGraphName) && graphs.has(secondGraphName) && !graphs.has(resultGraphName))
  {
    Graph result = merge(graphs.at(firstGraphName), graphs.at(secondGraphName));
    graphs.add(resultGraphName, result);
  }
  else
  {
    throw std::runtime_error("No graph with this name or result graph already exists");
  }
}

void tarasenko::extractGraph(std::istream& in, std::ostream&, GraphTable& graphs)
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
    Graph result = graphs.at(graphName).extract(vertexes);
    graphs.add(resultGraphName, result);
  }
  else
  {
    throw std::runtime_error("No graph with this name or result graph already exists");
  }
}
