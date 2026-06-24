#include "commands.hpp"
#include <algorithm>
#include <istream>
#include <ostream>
#include <stdexcept>

void tarasenko::showGraphs(std::istream&, std::ostream& out, GraphTable& graphs)
{
  if (graphs.empty())
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

void tarasenko::showVertexes(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  in >> graphName;
  if (graphs.has(graphName))
  {
    Vector< std::string > vertexes = graphs.at(graphName).getVertexes();
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
