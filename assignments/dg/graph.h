#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <memory>

namespace gdwg {

template<typename T>
// requires Relation<Less<T>, T>()
struct CompareByValue {
  bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const {
    return *lhs < *rhs;
  }
};

template<typename N, typename E>
class Graph {
 public:
 private:
  Class Node;
  Class Edge;


  std::set<std::shared__ptr<Node>, CompareByValue<Node>> nodes_;



  class Node {
   public:
    //Set of shared pointers to edges
    std::set<std::shared__ptr<Node>, CompareByValue<Node>>;
   private:

  };


  class Edge {
   public:

   private:

  };

};

// Define your graph_iterator here
// Define your graph_const_iterator here

}

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_