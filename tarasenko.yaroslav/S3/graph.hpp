#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include "hash_table.hpp"
#include "vector.hpp"

namespace tarasenko
{
  struct EdgeInfo
  {
    std::string vertex;
    Vector< unsigned int > weights;
  };

  class Graph
  {
  public:
    void addVertex(const std::string& name);
    bool hasVertex(const std::string& name) const;

    void bind(const std::string& from, const std::string& to, unsigned int weight);
    bool cut(const std::string& from, const std::string& to, unsigned int weight);

    std::vector< std::string > getVertexes() const;
    std::vector< EdgeInfo > getOutbound() const;
    std::vector< EdgeInfo > getInbound() const;

    Graph extract(const Vector< std::string >& vertexes) const;

  private:
    HashTable<
      std::pair< std::string, std::string >,
      Vector< unsigned int >
    > edges_;

    HashTable< std::string, bool > vertexes_;
  };

  Graph merge(const Graph& lhs, const Graph& rhs);
}

#endif
