#include "graph.hpp"

namespace tarasenko
{
  void Graph::addVertex(const std::string& name)
  {
    vertexes_.add(name, true);
  }

  bool Graph::hasVertex(const std::string& name) const
  {
    return vertexes_.has(name);
  }

  void Graph::bind(const std::string& from, const std::string& to, unsigned int weight)
  {
    Graph copy = *this;

    copy.addVertex(from);
    copy.addVertex(to);

    std::pair< std::string, std::string > edge = {from, to};

    if (copy.edges_.has(edge))
    {
      Vector< unsigned int > weights = copy.edges_.get(edge);
      weights.pushBack(weight);
      copy.edges_.drop(edge);
      copy.edges_.add(edge, weights);
    }
    else
    {
      Vector< unsigned int > weights;
      weights.pushBack(weight);
      copy.edges_.add(edge, weights);
    }

    swap(copy);
  }

  void Graph::swap(Graph& rhs) noexcept
  {
    edges_.swap(rhs.edges_);
    vertexes_.swap(rhs.vertexes_);
  }

  bool Graph::cut(const std::string& from, const std::string& to, unsigned int weight)
  {
    if (!hasVertex(from) || !hasVertex(to))
    {
      return false;
    }

    std::pair< std::string, std::string > edge(from, to);
    if (!edges_.has(edge))
    {
      return false;
    }

    Graph copy = *this;
    Vector< unsigned int > weights = copy.edges_.get(edge);

    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      if (weights[i] == weight)
      {
        weights.erase(i);

        copy.edges_.drop(edge);
        if (!weights.isEmpty())
        {
          copy.edges_.add(edge, weights);
        }

        swap(copy);
        return true;
      }
    }

    return false;
  }

  Vector< std::string > Graph::getVertexes() const
  {
    Vector< std::string > result;
    for (auto it = vertexes_.begin(); it != vertexes_.end(); ++it)
    {
      result.pushBack(it->first);
    }
    return result;
  }

  Vector< EdgeInfo > Graph::getOutbound() const
  {
    Vector< EdgeInfo > result;
    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
      result.pushBack({it->first.second, it->second});
    }
    return result;
  }

  Vector< EdgeInfo > Graph::getInbound() const
  {
    Vector< EdgeInfo > result;
    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
      result.pushBack({it->first.first, it->second});
    }
    return result;
  }

  Graph Graph::extract(const Vector< std::string >& vertexes) const
  {
    Graph result;
    HashTable< std::string, bool > selected;

    for (auto it = vertexes.begin(); it != vertexes.end(); ++it)
    {
      selected.add(*it, true);
      result.addVertex(*it);
    }

    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
      const std::string& from = it->first.first;
      const std::string& to = it->first.second;
      if (selected.has(from) && selected.has(to))
      {
        const Vector< unsigned int >& edges = it->second;
        for (auto egdeIt = edges.begin(); egdeIt != edges.end(); ++egdeIt)
        {
          result.bind(from, to, *egdeIt);
        }
      }
    }

    return result;
  }

  Graph merge(const Graph& lhs, const Graph& rhs)
  {
    Graph result;

    for (auto it = lhs.vertexes_.begin(); it != lhs.vertexes_.end(); ++it)
    {
      result.addVertex(it->first);
    }
    for (auto it = rhs.vertexes_.begin(); it != rhs.vertexes_.end(); ++it)
    {
      result.addVertex(it->first);
    }

    for (auto it = lhs.edges_.begin(); it != lhs.edges_.end(); ++it)
    {
      const std::string& from = it->first.first;
      const std::string& to = it->first.second;
      const Vector< unsigned int >& edges = it->second;
      for (auto edgeIt = edges.begin(); edgeIt != edges.end(); ++edgeIt)
      {
        result.bind(from, to, *edgeIt);
      }
    }
    for (auto it = rhs.edges_.begin(); it != rhs.edges_.end(); ++it)
    {
      const std::string& from = it->first.first;
      const std::string& to = it->first.second;
      const Vector< unsigned int >& edges = it->second;
      for (auto edgeIt = edges.begin(); edgeIt != edges.end(); ++edgeIt)
      {
        result.bind(from, to, *edgeIt);
      }
    }

    return result;
  }
}
