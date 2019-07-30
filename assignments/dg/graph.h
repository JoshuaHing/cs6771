#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <memory>
#include <set>
#include <tuple>
#include <vector>

namespace gdwg {

template<typename N, typename E>
class Graph {
 public:

    Graph() = default;
    Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) noexcept;

    Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator tup_begin, typename std::vector<std::tuple<N, N, E>>::const_iterator tup_end) noexcept;



    bool InsertNode(const N& val);
    bool InsertEdge(const N& src, const N& dst, const E& w);
    bool IsNode(const N& val) const;

    ~Graph() = default;
 private:


  class Node;
  class Edge;


  std::set<std::shared_ptr<Node>> nodes_;



  class Node {
   public:

    Node() = default;
    Node(N value): value_{value} {}

    N GetValue() { return value_; }
    bool AddEdge(std::shared_ptr<Node> src, std::shared_ptr<Node> dst, const E& w);

    ~Node() = default;



   private:
    N value_;
    std::set<std::shared_ptr<Edge>> edges_;

    bool operator <(const Node &compare) const {
        return value_ < compare.value;
    }
  };


  class Edge {
   public:

    Edge() = delete;
    Edge(std::weak_ptr<Node> source, std::weak_ptr<Node> dest, E weight): source_{source}, dest_{dest}, weight_{weight} {}

    E GetWeight() { return weight_; }
    std::weak_ptr<Node> GetDest() { return dest_; }

    ~Edge() = default;

   private:
    std::weak_ptr<Node> source_;
    std::weak_ptr<Node> dest_;
    E weight_;

    bool operator <(const Edge &compare) const {
        return weight_ < compare.weight;
    }

  };

};

// Define your graph_iterator here
// Define your graph_const_iterator here

}

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_